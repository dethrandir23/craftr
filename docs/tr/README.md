# Craftr 🛠️

[](https://www.google.com/search?q=https://github.com/dethrandir23/craftr)
[](https://opensource.org/licenses/MIT)
[](https://isocpp.org/std/the-standard)

**Craftr**, projelerinizi sıfırdan oluşturma zahmetini ortadan kaldıran, şablon tabanlı güçlü bir komut satırı aracıdır. Her yeni projede klasörleri manuel olarak oluşturmaktan, standart dosyaları kopyalamaktan, build sistemlerini ayarlamaktan ve lisans eklemekten sıkıldınız mı? Craftr, tüm bu süreci sizin için otomatikleştiren esnek bir iskelet motorudur.

Başlangıçta bir C++ proje oluşturucu olarak doğmuş olsa da, **YAML tabanlı şablon motoru** sayesinde artık **herhangi bir dil veya proje yapısı** için anında başlangıç şablonları oluşturabilirsiniz.

## ✨ Özellikler

  - **Şablon Odaklı:** Kendi proje yapılarınızı YAML ile tanımlayın. C++, Python, Web veya aklınıza gelen herhangi bir şey için şablonlar oluşturun.
  - **Etkileşimli Kurulum:** `craftr` size şablonunuzdaki değişkenleri (`Proje Adı`, `Yazar` vb.) sorar, uzun komutları ezberlemenize gerek kalmaz.
  - **Tamamen Özelleştirilebilir:** Kendi şablonlarınızı `~/.config/craftr` dizini altına ekleyerek `craftr`'ı kişisel proje başlatıcınız haline getirin.
  - **Dinamik İçerik:** Şablon dosyalarınızdaki `{{PLACEHOLDER}}` gibi yer tutucular, verdiğiniz girdilerle otomatik olarak doldurulur.
  - **Platform Bağımsız:** Linux, macOS ve Windows üzerinde çalışmak üzere tasarlanmıştır.
  - **Oluşturma Sonrası Komutlar (Hooks):** Projeniz oluşturulduktan sonra git init veya npm install gibi kabuk komutlarını otomatik olarak çalıştırın.
## 📦 Kurulum

### Ön Gereksinimler

  - C++17 destekli bir derleyici (GCC, Clang, MSVC)
  - CMake 3.15 veya daha üstü

### Kaynaktan Derleme

```bash
# Depoyu klonla
git clone https://github.com/dethrandir23/craftr.git
cd craftr

# Build dizini oluştur ve derle
mkdir build && cd build
cmake ..
make

# Sisteme kur (isteğe bağlı)
sudo make install
```

## 🚀 Hızlı Başlangıç

Yeni bir proje oluşturmak hiç bu kadar kolay olmamıştı. `cpp` şablonunu kullanarak bir proje başlatalım:

```bash
craftr --create --template cpp
```

Craftr size proje detaylarını soracaktır:

```
Enter the PROJECT_NAME : Mordor
Enter the AUTHOR_NAME : Sauron
Select your license type:
- apache
- bsd
- gpl
- mit
Selected license (if you don't want one, press enter): mit
Project 'Mordor' created successfully!
```

Ve işte sonuç\! `Mordor` adında, belirttiğiniz detaylarla doldurulmuş, tam teşekküllü bir proje oluşturuldu:

```
Mordor/
├── CMakeLists.txt
├── include/
├── LICENSE
├── metadata/
│   └── metadata.json
├── README.md
└── src/
    └── main.cpp
```

## 🎨 Şablon Sistemi Nasıl Çalışır?

Craftr'ın kalbi, YAML ile tanımlanan şablon sistemidir. Bir şablon, proje yapısını, klasörleri, dosyaları, komutları ve kullanıcıdan istenecek değişkenleri tanımlar. İşte dahili `cpp` şablonunun `template.yaml` dosyası:

```yaml
name: "cpp"
version: "1.0.0"
author: "system"
description: "Default C++ project template"

# Bu değişkenler, proje oluşturulurken kullanıcıya sorulur
variables:
  PROJECT_NAME: "{{PROJECT_NAME}}"
  AUTHOR_NAME: "{{AUTHOR_NAME}}"
  DATE: "{{DATE}}" # DATE gibi bazı değişkenler sistem tarafından otomatik doldurulabilir

# Oluşturulacak alt klasörler
subfolders:
  - include
  - src
  - metadata

# Oluşturulacak dosyalar ve kullanılacak şablonlar
files:
  - target: "CMakeLists.txt"
    template: "cmake/CMake_Template.txt"
  - target: "src/main.cpp"
    template: "cpp/main.txt"
  - target: "README.md"
    template: "readme/Readme.txt"
  - target: "LICENSE"
    template: "license" # 'license' özel bir anahtar kelimedir
  - target: "metadata/metadata.json"
    template: "metadata/metadata.txt"

command_mode: cautious # Dikkatli mod, komutlardan biri basarisiz olursa yürütmeyi durdurur, ancak execute_all modu ne olursa olsun hepsini yürütür.

# Proje oluşturulduktan sonra yürütülecek komutlar
commands:
  - "cd {{PROJECT_NAME}} && git init"
  - "cd {{PROJECT_NAME}} && git add ."
  - "cd {{PROJECT_NAME}} && git commit -m 'Initial commit'"
```

Craftr, şablon dosyalarındaki (`CMake_Template.txt` vb.) `{{PROJECT_NAME}}` gibi yer tutucuları kullanıcının girdiği değerlerle değiştirir.

## 🧑‍🎨 Kendi Şablonlarınızı Oluşturun

Craftr'ın gerçek gücünü ortaya çıkarmak için kendi şablonlarınızı oluşturun\!

1.  Şablonlarınız için ana dizini oluşturun:

    ```bash
    mkdir -p ~/.config/craftr/templates
    ```

2.  Yeni şablonunuz için bir klasör oluşturun. Örneğin, bir Python projesi için `python-api`:

    ```bash
    mkdir ~/.config/craftr/templates/python-api
    ```

3.  Bu klasörün içine `template.yaml` dosyanızı ve `main.py.txt`, `requirements.txt.txt` gibi şablon kaynak dosyalarınızı yerleştirin.

Artık kendi şablonunuzu kullanarak yeni bir proje başlatabilirsiniz:

```bash
craftr --create --template python-api
```

## 🤝 Katkıda Bulunma

Katkılarınızı bekliyoruz\! Lütfen `issue` açmaktan, özellik talep etmekten veya `pull request` göndermekten çekinmeyin.

1.  Bu depoyu fork'layın.
2.  Yeni bir özellik dalı oluşturun (`git checkout -b feature/harika-bir-ozellik`).
3.  Değişikliklerinizi commit'leyin (`git commit -m 'Harika bir özellik eklendi'`).
4.  Dalınızı push'layın (`git push origin feature/harika-bir-ozellik`).
5.  Bir Pull Request açın.

## 📄 Lisans

Bu proje MIT Lisansı altında lisanslanmıştır. Detaylar için [LICENSE](https://www.google.com/search?q=LICENSE) dosyasına göz atın.

## 🙏 Teşekkürler

  - `cargo new` ve `cookiecutter` gibi harika araçlardan ilham alındı.
  - Komut satırı işlemleri için [Cliopatra](https://github.com/dethrandir23/Cliopatra) kütüphanesi kullanıldı.
  - Craftr'ı geliştirmeye yardımcı olan tüm katkıda bulunanlara teşekkürler\!

-----

**Craftr**, ❤️ ile [dethrandir23](https://github.com/dethrandir23) tarafından geliştirilmektedir.