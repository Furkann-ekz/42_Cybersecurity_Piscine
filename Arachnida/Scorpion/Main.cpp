#include <iostream>
#include "./Classes/Jpeg_Jpg.hpp"
// AllClasses.hpp içindeki renk tanımlarını (RED, YELLOW, RESET) kullanır.

int main(int argc, char **argv)
{
    // 1. Argüman Kontrolü
    if (argc != 2)
    {
        // Hata mesajı Kırmızı
        std::cerr << RED << "Error: Please enter a file path." << RESET << std::endl;
        std::cerr << "Usage: ./scorpion <file.jpg>" << std::endl;
        return (1);
    }

    // 2. Parser Nesnesinin Oluşturulması
    Jpeg_Jpg parser(argv[1]);

    // 3. Başlık (Sarı)
    std::cout << YELLOW << "----------------------------------------" << RESET << std::endl;
    std::cout << YELLOW << "FILE: " << argv[1] << " BEING ANALYZED..." << RESET << std::endl;
    std::cout << YELLOW << "----------------------------------------" << RESET << std::endl;

    // 4. Parse İşlemi
    parser.parse();

    // 5. Sonuçların Ekrana Yazdırılması (Renkli)
    // Artık döngü kurmuyoruz, AllClasses içindeki fonksiyonu çağırıyoruz.
    parser.print_data();

    // 6. Kapanış Çizgisi (Sarı)
    std::cout << YELLOW << "----------------------------------------" << RESET << std::endl;

    return (0);
}