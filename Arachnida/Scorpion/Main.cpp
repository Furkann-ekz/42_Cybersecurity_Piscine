#include <iostream>
#include <string>
#include "./Classes/Jpg.hpp"
#include "./Classes/Bmp.hpp"
#include "./Classes/Tif.hpp"
#include "./Classes/Png.hpp"
// Renk kodları AllClasses.hpp üzerinden geliyor (RED, YELLOW, RESET vs.)

int main(int argc, char **argv)
{
    // 1. Argüman Kontrolü
    if (argc != 2)
    {
        std::cerr << RED << "Hata: Lutfen bir dosya yolu giriniz." << RESET << std::endl;
        std::cerr << "Kullanim: ./scorpion <dosya_adi>" << std::endl;
        return (1);
    }

    std::string file_path = argv[1];
    std::string ext = "";
    
    // Uzantıyı bulma işlemi
    size_t dot_pos = file_path.find_last_of(".");
    if (dot_pos != std::string::npos)
        ext = file_path.substr(dot_pos + 1);

    // 2. Polimorfizm: Base Class Pointer
    AllClasses *parser = NULL;

    // 3. Uzantıya Göre Sınıf Seçimi
    if (ext == "jpg" || ext == "jpeg" || ext == "JPG" || ext == "JPEG")
    {
        parser = new JpegJpg(file_path);
    }
    else if (ext == "tif" || ext == "tiff" || ext == "TIF" || ext == "TIFF")
    {
        parser = new TiffTif(file_path);
    }
    else if (ext == "bmp" || ext == "BMP")
        parser = new Bmp(file_path);
    else if (ext == "png" || ext == "PNG")
        parser = new Png(file_path);
    else
    {
        std::cerr << RED << "Hata: Desteklenmeyen dosya formati (" << ext << ")." << RESET << std::endl;
        std::cerr << "Lutfen .jpg veya .tiff uzantili dosya giriniz." << std::endl;
        return (1);
    }

    // 4. Analiz Başlatılıyor (Sarı Başlık)
    std::cout << YELLOW << "----------------------------------------" << RESET << std::endl;
    std::cout << YELLOW << "DOSYA: " << file_path << " ANALIZ EDILIYOR..." << RESET << std::endl;
    std::cout << YELLOW << "----------------------------------------" << RESET << std::endl;

    // 5. Parse ve Yazdırma (Polimorfik Çağrı)
    // Hangi sınıfın parse fonksiyonu çalışacaksa o devreye girer.
    parser->parse();
    parser->print_data();

    std::cout << YELLOW << "----------------------------------------" << RESET << std::endl;

    // 6. Temizlik
    delete parser;

    return (0);
}