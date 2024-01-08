#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TABLO_BOYUTU 10

typedef struct {
    int takipNo;
    char gonderici[100];
    char alici[100];
    char kargoDurumu[100];
} Kargo;

Kargo* kargoTablo[TABLO_BOYUTU];

int hashIndex1(int takip) {
    return takip % TABLO_BOYUTU;
}

int hashIndex2(int takip) {
    return 7 - (takip % 7);
}

int doubleHashing(int takip, int deneme) {
    return (hashIndex1(takip) + deneme * hashIndex2(takip)) % TABLO_BOYUTU;
}

void ekle(int takipNo, const char* gonderici, const char* alici, const char* kargoDurumu) {
    int index = hashIndex1(takipNo), deneme = 1;
    Kargo* gecici = (Kargo*)malloc(sizeof(Kargo));

    while (kargoTablo[index] != NULL) {
        index = doubleHashing(takipNo, deneme);
        ++deneme;

        if (deneme >= TABLO_BOYUTU) {
            printf("TABLO DOLDU..!\n");
            free(gecici);
            return;
        }
    }

    gecici->takipNo = takipNo;
    strcpy(gecici->gonderici, gonderici);
    strcpy(gecici->alici, alici);
    strcpy(gecici->kargoDurumu, kargoDurumu);

    kargoTablo[index] = gecici;


}


void kullaniciKargoGirisi() {
    int takipNo;
    char gonderici[100];
    char alici[100];
    char kargoDurumu[100];

    printf("Lutfen gondereni giriniz..: ");
    getchar();
    fgets(gonderici, sizeof(gonderici), stdin);
    gonderici[strcspn(gonderici, "\n")] = '\0';

    printf("Lutfen aliciyi giriniz..: ");
    fgets(alici, sizeof(alici), stdin);
    alici[strcspn(alici, "\n")] = '\0';

    printf("Lütfen teslimat durum bilgisi giriniz..: ");
    fgets(kargoDurumu, sizeof(kargoDurumu), stdin);
    kargoDurumu[strcspn(kargoDurumu, "\n")] = '\0';

    printf("Lütfen takip numarasi giriniz..:\n");
    scanf("%d", &takipNo);

    ekle(takipNo, gonderici, alici, kargoDurumu);
    // Dosyaya yazma işlemi
    FILE* dosya = fopen("veriler.txt", "a");
    if (dosya == NULL) {
        printf("Dosya acilamadi..!\n");
        return;
    }

    fprintf(dosya, "\n%d %s %s %s", takipNo, gonderici, alici, kargoDurumu);

    fclose(dosya);
}

void kargoTakipDurum() {
    int takip, index, deneme = 1;
    int bulundu = 0;// Eğer kargo bulunduysa 1, bulunamadıysa 0

    printf("Lutfen durumunu ogrenmek istediginiz kargonun takip numarasini giriniz..:\n");
    scanf("%d", &takip);

    index = hashIndex1(takip);
    while (kargoTablo[index] != NULL) {
        if (kargoTablo[index]->takipNo == takip) {
            printf("Gonderici: %s   Alici: %s  Gonderi Durumu: %s\nAdim sayisi: %d\n ", kargoTablo[index]->gonderici, kargoTablo[index]->alici, kargoTablo[index]->kargoDurumu, deneme);
            bulundu = 1;
            break;
        }


        index = doubleHashing(takip, deneme);
        deneme++;
    }
    if (!bulundu) {
        printf("BULUNAMADI\n");
    }




}

void kargolariListele() {
    printf("\n...Tum kargolar indexteki siralarina gore listeleniyor...\n");
    for (int i = 0; i < TABLO_BOYUTU; i++) {
        if (kargoTablo[i] != NULL) {
            printf("Index: %d  Takip No: %d  Gonderici: %s\nAlici: %s  Kargo Durumu: %s\n", i, kargoTablo[i]->takipNo, kargoTablo[i]->gonderici, kargoTablo[i]->alici, kargoTablo[i]->kargoDurumu);
        }
    }
}

void kargoTeslimi() {
    int takip, index, deneme = 1;
    printf("Lutfen teslim edilmesini istediginiz kargonun takip numarasini giriniz..:");
    scanf("%d", &takip);
    index = hashIndex1(takip);

    while (kargoTablo[index] != NULL) {
        if (kargoTablo[index]->takipNo == takip) {
            free(kargoTablo[index]);
            kargoTablo[index] = NULL;
            printf("Kargonuz teslim edilmistir ve tablodan silinmistir.\n");
            return;
        }
        index = doubleHashing(takip, deneme);
        deneme++;
    }
    printf("Aranan kargo bulunamadi\n");
}

void kargonunTextGirisi() {
    FILE* fp = fopen("veriler.txt", "r+");
    if (fp == NULL) {
        printf("Dosya acilamadi..!\n");
        return;
    }

    int takipNo;
    char gonderen[100], alici[100], kargoDurumu[100];

    while (fscanf(fp, "%d %s %s %s", &takipNo, gonderen, alici, kargoDurumu) == 4) {
        ekle(takipNo, gonderen, alici, kargoDurumu);
    }

    fclose(fp);
}

int main() {
    int sorgu = 10;
    while (sorgu != 5) {
        printf("Asagıdaki islemlerden birini seciniz");
        printf("\n0 - Textten Veri Girisi Yap\n\n1 - Tum Kargolari Listele\n\n2 - Kargo Girisi \n\n3 - Kargo Takibi \n\n4 - Kargoyu Teslim Et\n\n5 - Cikis \n\n");
        scanf("%d", &sorgu);
        switch (sorgu) {
            case 0: kargonunTextGirisi(); break;
            case 1: kargolariListele(); break;
            case 2: kullaniciKargoGirisi(); break;
            case 3: kargoTakipDurum(); break;
            case 4: kargoTeslimi(); break;
            case 5: return 0;
            default: printf("Gecersiz bir islem yaptiniz\n");
        }
    }

    return 0;
}

