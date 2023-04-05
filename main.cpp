#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;



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
        unsigned int raster_bit = 0;


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
                filesize = filesize | (unsigned short)d<<(8*i);
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
                pixel_offset = pixel_offset | (unsigned short)d<<(8*i);
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
            cout<<"Размер файла: "<< file.filesize<<" байт ~= "<<file.filesize/(1024)<<" килобайт\n";
            cout<<"Смещение пиксельных данных: "<<file.pixel_offset<<"\n";

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
