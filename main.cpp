#include <iostream>
#include <fstream>
#include <windows.h>
#include <bitset>

using namespace std;

enum BMPVersion{
    CORE, v3, v4, v5
};


class BMParser{
    private:
        string filename;



        int getFileSize(){
            FILE* file;
            file = fopen(filename.c_str(), "rb");
            //определяем размер файла
            fseek(file, 0, SEEK_END);
            int filesize = ftell(file);
            fseek(file, 0, SEEK_SET);
            fclose(file);
            return filesize;
        }

    public:
        unsigned int filesize=0;
        unsigned int pixel_offset = 0;
        unsigned int width = 0;
        unsigned int height = 0;
        unsigned int planes = 0; //что бы оно не значило
        unsigned int raster_bit = 0;

        //v3
        unsigned int compression = 0;
        unsigned int SizeImage=0;
        unsigned int XPelsPerMeter=0;
        unsigned int YPelsPerMeter=0;
        unsigned int ClrUsed=0;
        unsigned int ClrImportant=0;

        //v4
        unsigned int RedMask=0;
        unsigned int GreenMask=0;
        unsigned int BlueMask=0;
        unsigned int AlphaMask=0;
        unsigned int CSType=0;

        unsigned int GammaRed=0;
        unsigned int GammaGreen=0;
        unsigned int GammaBlue=0;

        //v5
        unsigned int Intent=0;
        unsigned int ProfileData=0;
        unsigned int ProfileSize=0;
        unsigned int Reserved=0;



        BMPVersion ver;


        BMParser(string _filename){
            filename = std::move(_filename);
        }
        bool parse(){
            ifstream file(filename.c_str(), ios::in | ios::binary);
            char b;
            file.get(b);

            switch (b) {
                case 'B':
                    file.get(b);
                    if (b!='M')
                        return false;
                    break;

                case 'M':
                    file.get(b);
                    if (b!='B')
                        return false;
                    break;
            }

            for (int i =0; i<4;i++){
                file.get(b);
                unsigned char d = b;
                filesize = filesize | (unsigned int)d<<(8*i);
            }
            for (int i =0; i<4;i++){
                file.get(b);
                if (b!=0){
                    return false;
                }
            }
            for (int i =0; i<4;i++){
                file.get(b);
                unsigned char d = b;
                pixel_offset = pixel_offset | (unsigned int)d<<(8*i);
            }
            unsigned int verget=0;

            for (int i =0; i<4;i++){
                file.get(b);
                unsigned char d = b;
                verget = verget | (unsigned int)d<<(8*i);
            }

            switch (verget){
                case 12:
                    ver=CORE;
                    break;
                case 40:
                    ver=v3;
                    break;
                case 108:
                    ver=v4;
                    break;
                case 124:
                    ver=v5;
                    break;
                default:
                    return false;
            }
            if (ver==CORE){
                //ширина
                for (int i =0; i<2;i++){
                    file.get(b);
                    unsigned char d = b;
                    width = width | (unsigned short)d<<(8*i);
                }

                //высота
                for (int i =0; i<2;i++){
                    file.get(b);
                    unsigned char d = b;
                    height = height | (unsigned short)d<<(8*i);
                }
                //плэйнс (видимо значение можно ли поставить как курсор)

                for (int i =0; i<2;i++){
                    file.get(b);
                    unsigned char d = b;
                    planes = planes | (unsigned short)d<<(8*i);
                }
                if (!planes){
                    return false;
                }

                for (int i =0; i<2;i++) {
                    file.get(b);
                    unsigned char d = b;
                    raster_bit = raster_bit | (unsigned short) d << (8 * i);
                }
            }
            else{
                //ширина
                for (int i =0; i<4;i++){
                    file.get(b);
                    unsigned char d = b;
                    width = width | (unsigned int)d<<(8*i);
                }

                //высота
                for (int i =0; i<4;i++){
                    file.get(b);
                    unsigned char d = b;
                    height = height | (unsigned int)d<<(8*i);
                }

                //плэйнс (видимо значение можно ли поставить как курсор)
                for (int i =0; i<2;i++){
                    file.get(b);
                    unsigned char d = b;
                    planes = planes | (unsigned short)d<<(8*i);
                }
                if (!planes){
                    return false;
                }

                //Битность
                for (int i =0; i<2;i++) {
                    file.get(b);
                    unsigned char d = b;
                    raster_bit = raster_bit | (unsigned short) d << (8 * i);
                }

                //Сжатие
                for (int i =0; i<4;i++){
                    file.get(b);
                    unsigned char d = b;
                    compression = compression | (unsigned int)d<<(8*i);
                }

                //Пиксельные данные
                for (int i =0; i<4;i++){
                    file.get(b);
                    unsigned char d = b;
                    SizeImage = SizeImage | (unsigned int)d<<(8*i);
                }

                //Пиксель метр X
                for (int i =0; i<4;i++){
                    file.get(b);
                    unsigned char d = b;
                    XPelsPerMeter = XPelsPerMeter | (unsigned int)d<<(8*i);
                }

                //Пиксель метр Y
                for (int i =0; i<4;i++){
                    file.get(b);
                    unsigned char d = b;
                    YPelsPerMeter = YPelsPerMeter | (unsigned int)d<<(8*i);
                }

                //Таблица цветов в ячейках
                for (int i =0; i<4;i++){
                    file.get(b);
                    unsigned char d = b;
                    ClrUsed = ClrUsed | (unsigned int)d<<(8*i);
                }

                //Колво ячеек в таблице
                for (int i =0; i<4;i++){
                    file.get(b);
                    unsigned char d = b;
                    ClrImportant = ClrImportant | (unsigned int)d<<(8*i);
                }
                if (ver>=v4){
                    for (int i =0; i<4;i++){
                        file.get(b);
                        unsigned char d = b;
                        RedMask = RedMask | (unsigned int)d<<(8*i);
                    }
                    for (int i =0; i<4;i++){
                        file.get(b);
                        unsigned char d = b;
                        GreenMask = GreenMask | (unsigned int)d<<(8*i);
                    }

                    for (int i =0; i<4;i++){
                        file.get(b);
                        unsigned char d = b;
                        BlueMask = BlueMask | (unsigned int)d<<(8*i);
                    }
                    for (int i =0; i<4;i++){
                        file.get(b);
                        unsigned char d = b;
                        AlphaMask = AlphaMask | (unsigned int)d<<(8*i);
                    }
                    //Пропускаем эндпоинты

                    for (int i =0; i<36;i++){
                        file.get(b);
                    }

                    for (int i =0; i<4;i++){
                        file.get(b);
                        unsigned char d = b;
                        GammaRed = GammaRed | (unsigned int)d<<(8*i);
                    }

                    for (int i =0; i<4;i++){
                        file.get(b);
                        unsigned char d = b;
                        GammaGreen = GammaGreen | (unsigned int)d<<(8*i);
                    }

                    for (int i =0; i<4;i++){
                        file.get(b);
                        unsigned char d = b;
                        GammaBlue = GammaBlue | (unsigned int)d<<(8*i);
                    }
                    if (ver==v5){
                        for (int i =0; i<4;i++){
                            file.get(b);
                            unsigned char d = b;
                            Intent = Intent | (unsigned int)d<<(8*i);
                        }
                        for (int i =0; i<4;i++){
                            file.get(b);
                            unsigned char d = b;
                            ProfileData = ProfileData | (unsigned int)d<<(8*i);
                        }
                        for (int i =0; i<4;i++){
                            file.get(b);
                            unsigned char d = b;
                            ProfileSize = ProfileSize | (unsigned int)d<<(8*i);
                        }
                        for (int i =0; i<4;i++){
                            file.get(b);
                            unsigned char d = b;
                            Reserved = Reserved | (unsigned int)d<<(8*i);
                        }
                    }











                }










            }





            if (file.eof()){
                file.close();
                return false;
            }

            return 1;
        }

};



int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    if (argc==2){
        auto file = BMParser(argv[1]);
        if (file.parse()){
            cout<<"Файл успешно прочитан\n";
            switch (file.ver){
                case CORE:
                    cout<<"Версия BMP: CORE";
                    break;
                case v3:
                    cout<<"Версия BMP: 3";
                    break;
                case v4:
                    cout<<"Версия BMP: 4";
                    break;
                case v5:
                    cout<<"Версия BMP: 5";
                    break;
                default:
                    cout<<"Версия BMP: Не определена";
                    break;

            }
            cout<<"\n";
            cout<<"Размер файла: "<< file.filesize<<" байт ~= "<<file.filesize/(1024)<<" килобайт\n";
            cout<<"Смещение пиксельных данных: "<<file.pixel_offset<<"\n";
            cout<<"Ширина: "<<file.width<<" px\n";
            cout<<"Высота: "<<file.height<<" px\n";
            long long col=(long long)1<<(file.raster_bit);
            cout<<"Битность: "<<file.raster_bit<<"\n";
            cout<<"Цветов возможно: "<<col<<"\n";
            if (file.ver>=v3){
                if (!file.compression){
                    cout<<"Сжатие: не используется\n";
                }
                else{
                    cout<<"Сжатие: используется\n";
                }

                cout<<"Размер пиксельных данных: "<< file.SizeImage<<" байт ~= "<<file.SizeImage/(1024)<<" килобайт\n";
                cout<<"Кол-во пикселей на метр ширины: "<< file.XPelsPerMeter<<"\n";
                cout<<"Кол-во пикселей на метр высоты: "<< file.YPelsPerMeter<<"\n";
                cout<<"Размер таблицы цветов в ячейках: "<<file.ClrUsed<<"\n";
                cout<<"Количество ячеек от начала таблицы цветов: "<<file.ClrImportant<<"\n";
                if (file.ver>=v4){
                    cout<< "Битовая маска для красного канала: " <<bitset<32>(file.RedMask)<<"\n";
                    cout<< "Битовая маска для зелёного канала: " <<bitset<32>(file.GreenMask)<<"\n";
                    cout<< "Битовая маска для синего канала: " <<bitset<32>(file.BlueMask)<<"\n";
                    cout<< "Битовая маска для альфа канала: " <<bitset<32>(file.AlphaMask)<<"\n";
                }

            }




        }
        else{
            cout<<"Проблема при чтении файла!\n";
            return 1;
        }
    }
    else{
        cout<<"Неверные аргументы! Используйте:\n"
              ".\\BMParser.exe <Файл.bmp>";
    }
    return 0;
}
