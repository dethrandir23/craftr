[![Lisans: GPL](https://img.shields.io/badge/License-GPL-yellow.svg)](https://www.gnu.org/licenses/gpl-3.0.en.html)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![CMake Sürümü](https://img.shields.io/badge/CMake-3.15%2B-064F8C.svg)](https://cmake.org/)
[![Platform Desteği](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-blue.svg)](https://github.com/dethrandir23/craftr)

---

# Craftr 🛠️

[](https://opensource.org/licenses/MIT)
[](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[](https://cmake.org/)
[](https://github.com/dethrandir23/craftr)

**Craftr**, sıfırdan proje başlatma zahmetini ortadan kaldıran güçlü ve şablon tabanlı bir komut satırı aracıdır. Her seferinde klasör oluşturmak, hazır dosyaları kopyalamak, derleme sistemlerini ayarlamak ve lisans dosyaları eklemekten sıkıldınız mı? **Craftr**, esnek iskelet (scaffolding) motoruyla tüm bu süreci sizin için otomatik hale getirir.

Başlangıçta bir C++ proje oluşturucu olarak ortaya çıkmış olsa da, **YAML tabanlı şablon motoru** sayesinde artık **her dil veya proje yapısı** için hızlı başlangıç şablonları oluşturabilirsiniz.

## ✨ Özellikler

* **Şablon Tabanlı:** Kendi proje yapınızı YAML dosyalarıyla tanımlayın.
* **Etkileşimli Kurulum:** `craftr` sizden değişkenleri ister, uzun komutları ezberlemenize gerek kalmaz.
* **Oluşturma Sonrası Komutlar:** Proje oluşturulduktan sonra `git init` veya `npm install` gibi komutları otomatik çalıştırır.
* **Şablon Doğrulama:** `--validate` komutuyla şablonlarınızın hatasız ve kullanıma hazır olduğundan emin olun.
* **🌐 Uzak Şablonlar:** Git depolarından veya merkezi bir topluluk havuzundan şablonları çekin, hatta doğrudan uzak şablonlarla çalışın.
* **🧙‍♂️ Klasörden Şablon:** Var olan herhangi bir proje dizinini anında Craftr şablonuna dönüştürün.
* **🌍 Çok Dilli Destek:** 15 dil desteği - İngilizce, Türkçe, İspanyolca, Fransızca, Almanca, İtalyanca, Portekizce, Rusça, Çince, Japonca, Korece, Arapça, Hintçe, Felemenkçe, Lehçe
* **Çapraz Platform:** Linux, macOS ve Windows üzerinde sorunsuz çalışacak şekilde tasarlanmıştır.

## 📦 Kurulum

### Ön Koşullar

* C++17 uyumlu bir derleyici (GCC, Clang, MSVC)
* CMake 3.15 veya üstü

### Kaynaktan Derleme

```bash
# Depoyu klonla
git clone https://github.com/dethrandir23/craftr.git
cd craftr

# Build klasörü oluştur ve derle
mkdir build && cd build
cmake ..
make

# Sistem genelinde kur (opsiyonel)
sudo make install
```

## 🚀 Kullanım

### Hızlı Başlangıç: Proje Oluşturma

Dahili `cpp` şablonunu kullanarak bir proje başlatalım:

```bash
craftr --create --template cpp
```

Craftr sizi adım adım yönlendirecektir:

```
Enter PROJECT_NAME: Mordor
Enter AUTHOR_NAME: Sauron
Select your license type:
- apache
- bsd
- gpl
- mit
Selected license (if you don't want one, press enter): mit
[INFO] Mordor için Git deposu başlatılıyor...
[OK] cd Mordor && git init
[INFO] Dosyalar ilk commit için hazırlanıyor...
[OK] cd Mordor && git add .
[INFO] İlk commit oluşturuluyor...
[OK] cd Mordor && git commit -m 'Initial commit'
[SUCCESS] Tüm komutlar başarıyla çalıştırıldı.
[SUCCESS] 'Mordor' projesi başarıyla oluşturuldu!
```

### Şablonları Yönetme

#### Şablon Doğrulama (`--validate`)

Şablonunuzun doğru biçimde olup olmadığını ve dosyaların eksiksiz bulunup bulunmadığını kontrol edin:

```bash
# Hatalı doğrulama örneği
$ craftr --validate --template noob
[FAIL] 'noob' şablonunda doğrulama hataları:
 - Gerekli alan eksik: version
 - Gerekli alan eksik: author
 - command_mode için geçersiz değer. "cautious" veya "execute_all" olmalı.

# Başarılı doğrulama örneği
$ craftr --validate --template cpp
[OK] 'cpp' şablonu geçerli ✅
```

#### Uzak Şablon Kullanımı

Craftr, Git depolarından doğrudan şablon kullanmanıza izin verir:

```bash
# 1. Tüm resmi topluluk şablonlarını çek
craftr --pull=template center

# 2. Belirli bir Git deposundan şablon çek
craftr --pull=template https://github.com/user/my-awesome-template

# 3. Uzak şablonu yerel olarak kaydetmeden doğrudan kullan
craftr --create --remote https://github.com/user/my-awesome-template
```

## 🧑‍🎨 Kendi Şablonunu Oluştur

### Kolay Yöntem: Klasörden Çıkarma (`--extract`)

Var olan bir projeyi şablona dönüştürmek en hızlı yöntemdir:

```bash
craftr --extract /path/to/your/existing-project
```

Craftr sizden birkaç bilgi alır ve otomatik olarak tam bir şablon yapısı oluşturur:

```
Klasörden şablon oluşturuluyor: /path/to/your/existing-project/
Yeni şablon için bir isim girin: my-first-template
Şablon sürümü (örn: 1.0): 1.0.0
...
[SUCCESS] 'my-first-template' şablonu başarıyla oluşturuldu!
```

### Manuel Yöntem

1. Kullanıcı şablonları için ana dizin oluşturun:
   `mkdir -p ~/.config/craftr/templates`
2. Yeni bir klasör açın:
   `mkdir ~/.config/craftr/templates/python-api`
3. İçine `template.yaml` ve kaynak dosyalarınızı (ör. `main.py.txt`) koyun.

## 🎨 Şablon Sistemi Açıklaması

Craftr’ın kalbi `template.yaml` dosyasıdır. Bu dosya proje yapısını, değişkenleri ve oluşturma sonrası komutları tanımlar.

```yaml
name: "cpp"
version: "1.0.0"
author: "system"
description: "Varsayılan C++ proje şablonu"

# Kullanıcıdan istenecek değişkenler
variables:
  PROJECT_NAME: "{{PROJECT_NAME}}"
  AUTHOR_NAME: "{{AUTHOR_NAME}}"
  DATE: "{{DATE}}" # DATE gibi bazı değişkenler otomatik doldurulabilir

# Oluşturulacak alt klasörler
subfolders:
  - include
  - src

# Şablonlardan oluşturulacak dosyalar
files:
  - target: "CMakeLists.txt"
    template: "cmake/CMake_Template.txt"
  - target: "src/main.cpp"
    template: "cpp/main.txt"
  - target: "LICENSE"
    template: "license" # 'license' özel bir anahtar kelime

# --- Oluşturma Sonrası Komutlar ---

# Seçenekler: 'cautious' (hata olursa durur) veya 'execute_all' (tümünü çalıştırır)
command_mode: cautious

# Komut çıktısını gizle, sadece Craftr’ın durum mesajlarını göster
silent_mode: true

# Proje oluşturulduktan sonra çalıştırılacak komutlar
commands:
  - command: "cd {{PROJECT_NAME}} && git init"
    description: "{{PROJECT_NAME}} için Git deposu başlatılıyor..."
  - command: "cd {{PROJECT_NAME}} && git add ."
    description: "İlk commit için dosyalar hazırlanıyor..."
  - command: "cd {{PROJECT_NAME}} && git commit -m 'Initial commit'"
    description: "İlk commit oluşturuluyor..."
```

## 🎯 Komut Referansı

### Hızlı Yardım Çıktısı

`craftr --help` komutunu çalıştırarak tüm kullanılabilir komutları görebilirsin:
bash

```
┌─ Craftr CLI Yardım ────────────────────────
│ Craftr, şablonlar kullanarak projeler oluşturmanı ve yönetmeni sağlayan bir araçtır.
│ 
│ Kullanım:
│   craftr [SEÇENEKLER] [KOMUT]
│ 
│ Temel Komutlar:
│   -h, --help              Yardımı göster ve çık
│   -v, --version           Sürüm bilgisini göster ve çık
│   -l, --language <locale> Dili değiştir (örn: 'tr', 'en', 'system')
│ 
│ Proje Oluşturma:
│   -c, --create            Yeni bir proje oluştur
│   -t, --template <isim>   Kullanılacak şablonu belirt
│   -f, --find              Şablonları ara ve etkileşimli olarak oluştur
│   -n, --name <isim>       find komutu için proje adını belirt
│ 
│ Şablon Yönetimi:
│   -li, --list [kategori]  Mevcut şablonları listele (kategoriler: all, user, center, remote, system)
│   -p, --pull <tip> <kaynak> Şablon veya lisans indir (örn: 'template center', 'template <url>')
│   -e, --extract <klasör>  Mevcut bir projeden şablon oluştur
│   -va, --validate          Şablon doğruluğunu kontrol et
│   -r, --remote <url>      Git deposundan uzak şablon kullan
│ 
│ Gelişmiş Komutlar:
│   -a, --add <modül>       Projeye modül veya bileşen ekle
│   -b, --build <hedef>     Projeyi derle (şablon destekliyorsa)
│   -co, --config <anahtar=değer> Yapılandırma seçeneklerini ayarla
│ 
│ Örnekler:
│   # Yeni bir C++ projesi oluştur
│   craftr --create --template cpp
│   # Şablon ara ve oluştur
│   craftr --find --name cpp
│   # Tüm şablonları listele
│   craftr --list
│   # Topluluk şablonlarını indir
│   craftr --pull template center
│   # Dili Türkçe yap
│   craftr --language tr
│ 
│ Daha fazla bilgi için dokümantasyona bakabilirsin.
└──────────────────────────────────────────
```

### Detaylı Komut Örnekleri

🔍 **Projeleri Bul ve Oluştur**

bash

```
### Etkileşimli şablon arama ve oluşturma
craftr --find --name cpp

### Belirli şablon ile oluştur
craftr --create --template cpp

### Uzak şablonu doğrudan kullan
craftr --create --remote https://github.com/user/template-repo
```

📁 **Şablon Yönetimi**

bash

```
### Tüm şablonları listele
craftr --list

### Belirli kategoriyi listele
craftr --list user
craftr --list center
craftr --list remote

### Şablon indir
craftr --pull template center
craftr --pull template https://github.com/user/template-repo

### Şablonu doğrula
craftr --validate --template cpp

### Mevcut projeden şablon oluştur
craftr --extract ./my-project
```

⚙️ **Yapılandırma**

bash

```
### Dili değiştir
craftr --language tr
craftr --language en
craftr --language system

### Sürümü göster
craftr --version
```

## 🤝 Katkıda Bulunma

Katkılarınızı bekliyoruz! Hata bildirimi, özellik isteği veya pull request gönderebilirsiniz.

1. Depoyu forklayın.
2. Yeni bir özellik dalı açın (`git checkout -b feature/amazing-feature`).
3. Değişikliklerinizi commit’leyin (`git commit -m 'Harika bir özellik ekle'`).
4. Dalı push edin (`git push origin feature/amazing-feature`).
5. Pull Request açın.

## 📄 Lisans

Bu proje **GPL Lisansı** ile lisanslanmıştır. Ayrıntılar için [LICENSE](../../LICENSE) dosyasına bakın.

## 🙏 Teşekkürler

* `cargo new` ve `cookiecutter` gibi harika araçlardan ilham alındı.
* Komut satırı ayrıştırma için [Cliopatra](https://github.com/dethrandir23/Cliopatra) kullanıldı.
* Şablon sistemi için [yaml-cpp](https://github.com/jbeder/yaml-cpp) kullanıldı.
* Craftr’ı geliştirmeye katkıda bulunan herkese teşekkürler!

---

## 🌐 Diğer Diller

* Lütfen ana [README.md](https://github.com/dethrandir23/craftr/blob/main/README.md) dosyasına göz atın.

**Craftr**, [dethrandir23](https://github.com/dethrandir23) tarafından ❤️ ile geliştirildi.
