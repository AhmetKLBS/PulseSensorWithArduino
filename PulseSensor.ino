#define ornekleme_boyutu 10
#define yukselis_esigi 4

int sensorPin = 0;

void setup() {
    Serial.begin(9600);
}

void loop ()
{
    float oku[ornekleme_boyutu], toplam;
    long int simdi, ptr;
    float son, okuyucu, baslat;
    float ilk, ikinci, ucuncu, onceki, degeri_yazdir;
    bool yukselis;
    int yukselis_sayisi;
    int n;
    long int son_nabiz;

    for (int i = 0; i < ornekleme_boyutu; i++)
      oku[i] = 0;
    toplam = 0;
    ptr = 0;

    while(1)
    {
      //20ms bekleme süresi ile 50Hz elektrik hattından gelen gürültüyü engeller
      n = 0;
      baslat = millis();
      okuyucu = 0.;
      do
      {
        okuyucu += analogRead (sensorPin);
        n++;
        simdi = millis();
      }
      while (simdi < baslat + 20);  
      okuyucu /= n;  // ortalama aldık
      
     //  diziye en yeni ölçümü ekle
       // ve diziden en eski ölçümü çıkar
       // toplam son ölçümlerini koru

      toplam -= oku[ptr];
      toplam += okuyucu;
      oku[ptr] = okuyucu;
      son = toplam / ornekleme_boyutu;
      // simdi dizideki değerlerin ortalamasını tutar

      // yukselis egrisini kontrol et
      if (son > onceki)
      {
        yukselis_sayisi++;
        if (!yukselis && yukselis_sayisi > yukselis_esigi)
        {
         //  bir kalp atışı anlamına gelen bir yukselis eğrisi tespit ettik.
           // son vuruştan bu yana geçen süreyi kaydeder, önceki ikiliyi takip eder
           //  (ilk, ikinci, ucuncu) ortalama alınır.
           // yükseliş kontrolu aynı yükselişi birden fazla algılamamızı engeller.
          yukselis = true;
          ilk = millis() - son_nabiz;
          son_nabiz = millis();

          // ortalamayi hesaplar
          degeri_yazdir = 60000. / (0.4 * ilk + 0.3 * ikinci + 0.3 * ucuncu);
          
          Serial.print(degeri_yazdir);
          Serial.print('\n');
          
          ucuncu = ikinci;
          ikinci = ilk;
          
        }
      }
      else
      {
        // egri dusuyor
        yukselis = false;
        yukselis_sayisi = 0;
      }
      onceki = son;
      
      
      ptr++;
      ptr %= ornekleme_boyutu;

    }
}