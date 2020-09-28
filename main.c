#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 20
#define MAX_DIR 150

//	Keyword'un hangisi olduðunu kararlaþtýrýp aþaðýdaki char arrayinde bulunduðu indexi commandFlag olarak döndürüyoruz.
int isKeyword(char storage[],int *commandFlag)
{
    char keywords[4][6] = {"if","ifnot","make","go"};

    int i;

    for (i = 0; i < 4; i++)
    {
        if (strcmp(keywords[i], storage) == 0)
        {
            *commandFlag = i;
            break;
        }
    }
    return *commandFlag;
}

// Make metodunun tüm iþlemleri burada yapiliyor.
int make(const char *dirStorage,int star_Count,int line)
{
    char directoryNames[256]; 					// Icice dosya olusturmak icin kullanilan degiskenler
    char *pointer = NULL;
    size_t len;
    char currentDir[MAX_DIR];					// Current Directoryi almak icin bir char arrayi
    char maxDir[] = "C:\\Users";				// En ust klasoru burada tanimladik
    char subdir[MAX_DIR];						// Make fonksiyonu current directoryi degistirmeyecegi icin en bastaki konumu yedeklemek icin subdir olusturuldu.
    char* temp;									// strtok metodunda NULL karakteriyle diger pointeri yakalamak icin kullanildi.


    strcpy(subdir,getcwd(currentDir, sizeof(currentDir)));		// Make konum degistirmedigi icin Current Directory yedekliyoruz

    int j =0;

    if(star_Count)								// Eger make fonksiyonu icerisinde ust klasor isareti * gelmisse burada islem yapilir.
    {
        for(j = 0 ; j<star_Count; j++)			// Burada * sayisi kadar ust klasore cikma islemi yapilir.
        {
            chdir("..");						// Ust klasore cikma islemi
            strtok(dirStorage,"/"); 			// strtok metodu ile / isaretine gore boluyoruz
            temp =strtok(NULL,"/");				// * isaretinden sonraki directory name'i tutar
			getcwd(currentDir, sizeof(currentDir));

            if(temp != NULL)
            {
                strcpy(dirStorage,temp);
            }
            if (strcmp(currentDir,maxDir) == 0)		// Max Directory'e ulasmissa hata yazdirir.
            {
                printf("Make Function Error! - Max Directory is %s so you cant go up anymore. (Line %d)\n",maxDir,line);
                break;
            }

        }
    }

	// Icice klasor olusturma metodu
    snprintf(directoryNames, sizeof(directoryNames),"%s",dirStorage);

    len = strlen(directoryNames);
    if(directoryNames[len - 1] == '/'){
    	directoryNames[len - 1] = 0;
	}
    for(pointer = directoryNames + 1; *pointer; pointer++){
    	if(*pointer == '/')
        {
            *pointer = 0;
            mkdir(directoryNames);
            *pointer = '/';
        }
	}

	// Dosya olusturma islemi burada yapilir. Eger dosya zaten varsa hata verilir.
    if(mkdir(directoryNames) == -1)
    {
        printf("Make Function Error! - Path already exits.\t\t(Line %d)\tPath\t\t\t: %s\\%s\n",line,getcwd(currentDir,sizeof currentDir),dirStorage);
        chdir(subdir);
        return -1;
    }

    printf("Make Function is finished successfully.\t\t\t(Line %d)\tPath\t\t\t: %s\\%s\n",line,getcwd(currentDir,sizeof currentDir),directoryNames);
    // Make konum degistirtmedigi icin eski konumumuza direkt geri donduk
	chdir(subdir);
}


// Go metodunun tum islemleri burada yazildi.
int go(char *dirStorage,int star_Count,int line)
{
    char maxDir[] = "C:\\Users";				// En ust klasoru burada tanimladik
    char currentDir[MAX_DIR];					// Current Directoryi almak icin bir char arrayi
    char subdir[MAX_DIR];						// Go fonksiyonu hataliysa current directoryi degistirmeyecegi icin en bastaki konumu yedeklemek icin subdir olusturuldu.
    char* temp;									// strtok metodunda NULL karakteriyle diger pointeri yakalamak icin kullanildi.
    int j = 0;

    strcpy(subdir,getcwd(currentDir, sizeof(currentDir)));

    if(star_Count)								// Eger go fonksiyonu icerisinde ust klasor isareti * gelmisse burada islem yapilir.
    {
        for(j = 0 ; j<star_Count; j++)			// Burada * sayisi kadar ust klasore cikma islemi yapilir.
        {
            chdir("..");						// Ust klasore cikma islemi
            strtok(dirStorage,"/"); 			// strtok metodu ile / isaretine gore boluyoruz
            temp =strtok(NULL,"/");				// * isaretinden sonraki directory name'i tutar
			getcwd(currentDir, sizeof(currentDir));

            if(temp != NULL)
            {
                strcpy(dirStorage,temp);
            }

            if (strcmp(currentDir,maxDir) == 0)		// Max Directory'e ulasmissa hata yazdirir.
            {
                printf("Go   Function Error! - Max Directory is %s so you cant go up anymore.\n",maxDir);
                break;
            }
        }
        if(chdir(dirStorage) != 0 && temp != NULL)
        {
            printf("Go   Function Error! - Path doesn't exits.\t\t(Line %d)\tPath\t\t\t: %s\\%s\n",line,getcwd(currentDir,sizeof currentDir),dirStorage);
            chdir(subdir);
            return -1;
        }
        printf("Go   Function is finished successfully.\t\t\t(Line %d)\tCurrent Directory\t: %s\n",line,getcwd(currentDir,sizeof currentDir));
        return 0;
    }

    else if(dirStorage != NULL)				// * isareti gelmemisse dosya degistirme islemi burada yapilir
    {
        if(chdir(dirStorage) != 0)
        {
        	printf("Go   Function Error! - Path doesn't exits.\t\t(Line %d)\tPath\t\t\t: %s\\%s\n",line,getcwd(currentDir,sizeof currentDir),dirStorage);
            return -1;
        }
        printf("Go   Function is finished successfully.\t\t\t(Line %d)\tCurrent Directory\t: %s\n",line,getcwd(currentDir,sizeof currentDir));
        return 0;
    }
}

// if metodunun tum islemleri burada yazilmistir
bool ifFunc(int star_Count, char* dirStorage)
{

    char maxDir[] = "C:\\Users";				// En ust klasoru burada tanimladik
    char currentDir[MAX_DIR];					// Current Directoryi almak icin bir char arrayi
    char subdir[MAX_DIR];						// If fonksiyonu current directoryi degistirmeyecegi icin en bastaki konumu yedeklemek icin subdir olusturuldu.
    char* temp;									// strtok metodunda NULL karakteriyle diger pointeri yakalamak icin kullanildi.
    int j = 0;

    strcpy(subdir,getcwd(currentDir, sizeof(currentDir)));

    if(star_Count)								// Eger if fonksiyonu icerisinde ust klasor isareti * gelmisse burada islem yapilir.
    {
        for(j = 0 ; j<star_Count; j++)			// Burada * sayisi kadar ust klasore cikma islemi yapilir.
        {
            chdir("..");
            strtok(dirStorage,"/");
            temp =strtok(NULL,"");
			getcwd(currentDir, sizeof(currentDir));

            if(temp != NULL)
            {
                strcpy(dirStorage,temp);
            }

            if (strcmp(currentDir,maxDir) == 0)
            {
                return false;					// maxDir'e ulasildigi icin false dondurulur. Boylece if'e bagli olan command ya da block calistirilmaz.
            }
        }

        //sadece yildiz gelirse for döngüsü bitiminde temp == NULL oluyor. dirStorage içerisinde de son yildiz kaliyor.

        if(temp != NULL && (chdir(dirStorage))==0)	// Dosya yolunun varligi chdir ile kontrol edilmektedir.
        {
            chdir(subdir);
            return true;						// Dosya yolu bulunmustur, boylece subdir icerisinde yedekledigimiz eski dizinimize doneriz ve true dondururuz. If'e bagli command ya da block calisir.
        }
        else if(temp == NULL && strcmp(dirStorage,"*") == 0)	// If icerisinde sadece ust klasor karakteri gelmisse kontrol burada yapilir.
        {
            chdir(subdir);
            return true;
        }
    }

    else if(dirStorage != NULL)
    {
        if(chdir(dirStorage) != 0)				// Dosya yolu bulunamadigi icin false dondurulur.
        {
            chdir(subdir);
            return false;
        }
        chdir(subdir);							// If dosya yolunu degistirmedigi icin dosya yolunun varligini ogrendikten sonra geri donuyoruz.
        return true;							// Dosya yolu bulundugu icin true donduruluyor.
    }
}




int main()
{
    FILE *source;								// Dosya okumak icin pointer
    char source_name[MAX_SIZE];					// Kullanicinin dosya adi girecegi icin olusturuldu
    char pmk[] = ".pmk";						// Dosyanin uzantisi .pmk olacagi icin olusturuldu
	printf("pmk file name : ");
    gets(source_name);
    strcat(source_name,pmk);					// Kullanicidan sadece dosya ismi alinir, devamina .pmk dosya uzantisini biz burada ekleriz.

    if((source = fopen(source_name,"r")) == NULL )		// Dosya okuma islemi
    {
        printf("%s","File is not found.");
        return -1;
    }

    // Current Directory alma islemi

    char currentDir[MAX_DIR];
    if (getcwd(currentDir, sizeof(currentDir)) != NULL)
    {
        printf("\nCurrent Directory at the beginning: %s\n\n", currentDir);
    }

    int lineCount = 1;							// Console uzerinde Line bilgisi yazdirmak icin olusturulan bir degisken

    int star_Count = 0;							// Directory Name'lerin icerisinde ne kadar ust klasor karakteri oldugunu saymak icin kullanilan bir degisken

    char oneLine[512] = {0};					// Dosyadan okunan bir satir bu degisken icerisinde depolanir

    char storage[32] = {0};						// Directory Name disindaki Keywordlerin ne oldugunu belirlemek amaciyla depolanmak icin kullanilan bir degisken
    int sCount = 0;								// Storage degiskeninin kac adet karakter barindirdigini tutan bir degisken

    int commandFlag = -1;						// Storage icerisinde biriken stringin hangi command oldugunu belirlemek icin kullanilan bir flag degeri

    bool ifFlag = false;						// isKeyword metodundan gelecek donuse gore if gelip gelmedigini anlamak icin kullanilan bir flag degeri
    bool ifnotFlag = false;						// isKeyword metodundan gelecek donuse gore ifnot gelip gelmedigini anlamak icin kullanilan bir flag degeri

    bool ifControl = false;						// ifFunc metodundan donecek degere gore if metodunun icerisindeki parametrenin hangi degerinde oldugunu burada tutariz. Ona gore if'e bagli olan komutlar calisir ya da calismaz.
    bool ifnotControl = false;					// ifFunc metodundan donecek degere gore ifnot metodunun icerisindeki parametrenin hangi degerinde oldugunu burada tutariz.  Ona gore ifnot'a bagli olan komutlar calisir ya da calismaz.

    bool bracket = false;						// if ve ifnot'tan sonra suslu parantez gelme durumunu burada kontrol ederiz. Eger suslu parantez gelmisse ve hala icerisindeysek bu degerimiz true'dur, cikinca false degerine doner.
    bool bracketOut = false;					// Eger ifFlag true ve ifControl false ise if'in altindaki tum block calismayacaktir. Bunu bu flag degeriyle yapiyoruz. Bu deger true ise o bracket icerisindeki hicbir islem yapilmaz. Bracket kapanincaya kadar atlamaya devam eder.

    char dirStorage[500] = {0};					// Directory Name'leri burada depoluyoruz.
    char  clean_Dir[MAX_DIR] = {0};				// Directory Name icerisinde bulunan(basinda veya sonunda bosluk bulunma durumu gibi) problem teskil etmeyen fazla karakterleri temizledikten sonra bu char arrayinde depoluyoruz. Bu arrayi diger metotlara yolluyoruz.
    int dirCount = 0;							// dirStorage icerisinde ne kadar deger varsa onlarin sayisini bu deger icerisinde tutuyoruz.
    bool dirFlag = false;						// < isareti gelmisse directory gelmis demektir, bu yuzden bu flagimiz true olur. > gelince false'a doner.

    bool isimKontrol = true;					// Eger directory name'lerinde herhangi bir problem yoksa bu deger true kalir. eger o an ki okunan directory name'inde problem varsa bu deger false olur ve hangi hata oldugu console ekranina yazdirilir.

	int i = 0;

    while(fgets(oneLine,sizeof oneLine,source))			// Satir satir okuma islemi burada yapilir.
    {


        for(i = 0; oneLine[i] != '\0' ; i++)			// Satirin her bir karakteri bu dongude okunur.
        {

            if((int)oneLine[i] == 125)					// Suslu parantezin bitme durumu
            {
                bracket = false;
                bracketOut = false;
                continue;
            }

            else if((int)oneLine[i] == 123)				// Suslu parantezin baslama durumu
            {
                bracket = true;
                continue;
            }

            else if(ifFlag && !ifControl && bracket)	// Eger if gelmisse ve parametresi dogru degilse o block icerisine girilmemesi icin bracketOut true yapilir.
            {
                bracketOut = true;
            }
        	else if(ifnotFlag && !ifnotControl && bracket)	// Eger ifnot gelmisse ve parametresi dogru ise o block icerisine girilmemesi icin bracketOut true yapilir.
            {
                bracketOut = true;
            }
            else if(bracketOut)							// bracketOut true ise block icerisindekileri isleme almamak icin surekli atlanir.
            {
                continue;
            }

            if((int)oneLine[i] == 60)					// < isareti gelmisse storage icerisinde kesinlikle bir keyword birikmistir. Onun ne oldugu burada bulunur.
            {
                isKeyword(storage,&commandFlag);
                dirFlag = true;
                memset(storage,0,sizeof(storage));
                sCount = 0;
                continue;
            }

            else if ((int)oneLine[i] == 62)				// > gelmisse directory name'ler okunmustur ve keyword de bellidir. Cogu islem burada yapilir.
            {
                dirFlag = false;

                // Directory name'leri / göre parçalama islemi burada yapilir.
                char * test;								// strtok icin olusturuldu.
                char * temp_Dir[strlen(dirStorage)];		// strtok metodunun dirStorage'i degistirmemesi icin bu degisken uzerinde islem yapilir.

                strcpy(temp_Dir,dirStorage);

                test = strtok (temp_Dir,"/");
                while (test != NULL)
                {

                	// Dosya isminin basinda veya sonunda bulunan bosluklar program syntaxinda hata vermezken, dosya gezme veya olusturma asamasinda hata verdigi icin directory name icerisindeki gereksiz bosluklari burada temizliyoruz.
                    int durum = -1, change = 0,ilkboslukicin = 0;

                    char* pointer1 = test;
                    char* pointer2 = test;

                    while(*pointer1 != 0)
                    {
                        if(*pointer1 != ' ')
                        {
                            *pointer2 = *pointer1;
                            pointer2++;
                            if(durum == 0)
                            {
                                if(*pointer1 == '/'){
                                    change = 0;
                                    durum = -1;
                                }
                                else{
                                change++;
                                durum = -1;
                                }
                            }
                        }
                        else if(*pointer1 == ' ' && ilkboslukicin != 0)
                        {
                            durum = 0;
                        }
                        ilkboslukicin++;
                        pointer1++;
                    }
                    *pointer2 = '\0';
                    if(change >= 1)
                    {
                        printf("Blank characters are not allowed in directory names.\t(Line %d)\tPath\t\t\t: ->%s<-\n",lineCount,dirStorage);
                        isimKontrol = false;

                    }
                    change = 0;
                    ilkboslukicin= 0;
					// Bosluk temizleme isleminin sonu

					int k = 0;
                    for (k = 0 ; k< strlen(test); k++)
                    {
                        // Standart hale getirmek için hepsini küçük harfe dönüstürüyoruz
                        test[k] = tolower(test[k]);

                        if(!(((int)test[k]>47)&& 58>(int)test[k]) && !(((int)test[k]>96)&& 123>(int)test[k]) && (int)test[k] != 95 && (int)test[k] != 42 && (int)test[k] != 32)
                        {
                            printf("Dir. names are made of letters, digits and underscore.\t(Line %d)\tPath\t\t\t: %s\n",lineCount,dirStorage);
                            isimKontrol = false;

                        }

                        if((int)test[k] == 42)	// * karakteri gelmisse star_Count degiskeni arttirilir.
                        {
                            star_Count++;
                        }
                    }

                    if(!(((int)test[0]>96)&& 123>(int)test[0]) && (int)test[0] != 42 && (int)test[0] != 32)
                    {
                        printf("Directory names start with a letter.\t\t\t(Line %d)\tPath\t\t\t: %s\n",lineCount,dirStorage);
                        isimKontrol = false;
                    }


                    strcat(clean_Dir,test);		// Directory name'de problem yoksa clean_Dir icerisine eklenir.
                    if((test= strtok (NULL, "/")) != NULL)
                    {
                        strcat(clean_Dir,"/");
                    }
                }



                // * Operatorunun yazim kontrolu
                char *e;
                int index;

                e = strchr(dirStorage, '*');
                index = (int)(e - dirStorage);
                if(e != NULL && index != 0)
                {
                    printf("Operator * can only be used at the beginning of path.\t(Line %d)\tPath\t\t\t: %s\n",lineCount,dirStorage);
                    isimKontrol = false;
                }



                // Basta / Operatorunun olma durumu

                e = strchr(dirStorage, '/');
                index = (int)(e - dirStorage);
                if(e != NULL && index == 0)
                {
                    printf("Operator / cannot be used at the beginning of path.\t(Line %d)\tPath\t\t\t: %s\n",lineCount,dirStorage);
                    isimKontrol = false;
                }

				// Sonda / Operatorunun olma durumu
                e = strrchr(dirStorage, '/');
                index = (int)(e - dirStorage);
                if(e != NULL && index+1 == strlen(dirStorage))
                {
                    printf("Operator / cannot be used at the end of path.\t\t(Line %d)\tPath\t\t\t: %s\n",lineCount,dirStorage);
                    isimKontrol = false;
                }


				// CommandFlag degeri bizim < isareti oncesinde hangi keywordun oldugunu soyler. Eger < isaretinden once if veya ifnot gelmisse kontrol islemleri burada yapilir.
                switch(commandFlag)
                {
                case 0:		// Eger < isaretinden once if gelmisse kontrolleri burada yapilir.
                    ifFlag = true;
                    ifControl = ifFunc(star_Count,clean_Dir);
                    memset(dirStorage, 0, sizeof(dirStorage));
                    memset(clean_Dir, 0, sizeof(clean_Dir));
                    dirCount = 0;
                    star_Count = 0;
                    break;
                case 1:		// Eger < isaretinden once ifnot gelmisse kontrolleri burada yapilir.
                    ifnotFlag = true;
                    ifnotControl = !(ifFunc(star_Count,clean_Dir));		// ifnotControl sadece ifFunc fonksiyonunun not'lanmasi ile kontrol edilir.
                    memset(dirStorage, 0, sizeof(dirStorage));
                    memset(clean_Dir, 0, sizeof(clean_Dir));
                    dirCount = 0;
                    star_Count = 0;
                    break;
                }

                continue;
            }

            else if(dirFlag)	// < isareti gelmisse dirFlag true olacagi icin o isaretten sonra okunan her deger dirStora icerisinde depolanir yani directory name'leri burada depoluyoruz.
            {
                dirStorage[dirCount] = oneLine[i];
                dirCount++;
                continue;
            }


            else if((int)oneLine[i] == 32 || oneLine[i] == '\t')	// Bosluk veya tab gelirse atliyoruz.
            {
                continue;
            }

            else if ((int)oneLine[i] == 59)				// ; isareti gelmisse ya make ya da go keywordu gelmistir. Onlarin islemleri burada yapilir.
            {
                if(!isimKontrol)						// Directory Namelerden herhangi biri yanlissa herhangi bir islem yapilmadan atlanir.
                {
                    isimKontrol = true;
                    memset(dirStorage, 0, sizeof(dirStorage));
                    memset(clean_Dir, 0, sizeof(clean_Dir));
                    dirCount = 0;
                    star_Count = 0;
                    continue;
                }

                if(ifFlag && ifControl)					// if gelmisse ve parametresi dogruysa yanindaki commandin veya block icerisindeki islemleri burada yapilir.
                {

                    switch(commandFlag)
                    {
                    case 2:		// ; isaretinden once make gelmisse islemleri burada yapilir.
                        make(&clean_Dir,star_Count,lineCount);
                        memset(dirStorage, 0, sizeof(dirStorage));
                        memset(clean_Dir, 0, sizeof(clean_Dir));
                        dirCount = 0;
                        star_Count = 0;
                        ifFlag = false;
                        ifControl = false;
                        break;
                    case 3:		// ; isaretinden once go gelmisse islemleri burada yapilir.
                        go(&clean_Dir,star_Count,lineCount);
                        memset(dirStorage, 0, sizeof(dirStorage));
                        memset(clean_Dir, 0, sizeof(clean_Dir));
                        dirCount = 0;
                        star_Count = 0;
                        ifFlag = false;
                        ifControl = false;
                        break;
                    }
                    continue;
                }

                else if(ifFlag && !ifControl)			// if gelmisse ve parametresi yanlissa yanindaki commandin veya block icerisindeki islemler burada atlanir.
                {
                    ifFlag = false;
                    continue;
                }

                else if(ifnotFlag && ifnotControl)		// ifnot gelmisse ve parametresi dogruysa yanindaki commandin veya block icerisindeki islemleri burada yapilir.
                {

                    switch(commandFlag)
                    {
                    case 2:		// ; isaretinden once make gelmisse islemleri burada yapilir.
                        make(&clean_Dir,star_Count,lineCount);
                        memset(dirStorage, 0, sizeof(dirStorage));
                        memset(clean_Dir, 0, sizeof(clean_Dir));
                        dirCount = 0;
                        star_Count = 0;
                        ifFlag = false;
                        ifControl = false;
                        break;
                    case 3:		// ; isaretinden once go gelmisse islemleri burada yapilir.
                        go(&clean_Dir,star_Count,lineCount);
                        memset(dirStorage, 0, sizeof(dirStorage));
                        memset(clean_Dir, 0, sizeof(clean_Dir));
                        dirCount = 0;
                        star_Count = 0;
                        ifFlag = false;
                        ifControl = false;
                        break;
                    }
                    continue;
                }

                else if(ifnotFlag && !ifnotControl)			// ifnot gelmisse ve parametresi yanlissa yanindaki commandin veya block icerisindeki islemler burada atlanir.
                {
                    ifnotFlag = false;
                    continue;
                }


                else if (!ifFlag && !ifnotFlag)				// Herhangi bir if ve ifnot degeri yoksa tum commandlerin islemleri burada yapilir.
                {
                    switch(commandFlag)
                    {
                    case 2:		// ; isaretinden once make gelmisse islemleri burada yapilir.
                        make(clean_Dir,star_Count,lineCount);
                        memset(dirStorage, 0, sizeof(dirStorage));
                        memset(clean_Dir, 0, sizeof(clean_Dir));
                        dirCount = 0;
                        star_Count = 0;
                        break;
                    case 3:		// ; isaretinden once go gelmisse islemleri burada yapilir.
                        go(&clean_Dir,star_Count,lineCount);
                        memset(dirStorage, 0, sizeof(dirStorage));
                        memset(clean_Dir, 0, sizeof(clean_Dir));
                        dirCount = 0;
                        star_Count = 0;
                        break;
                    }
                }
            }
            storage[sCount] = oneLine[i];	// Directory Name degilse yani Keyword ise storage icerisine depolama islemi burada yapilir.
            sCount++;						// Storage arrayinin total indexini tutmak icin olusturulan sCount degeri burada arttilir.
        }

        // Her satir sonunda depolanan ve sayilan tum degerler sifirlanir, boylece diger satirdaki degerlerle karisiklik yasanmaz.
        memset(dirStorage, 0, sizeof(dirStorage));
        memset(clean_Dir, 0, sizeof(clean_Dir));
        memset(storage, 0, sizeof(storage));
        sCount = 0;
        dirCount = 0;
        star_Count = 0;
        lineCount++;
    }
    return 0;
}
