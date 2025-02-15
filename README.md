# Post Ai Generated Images to Facebook Easily!

---

## Requirements

### **Installation Commands for Required Libraries and Tools**

```bash
git clone https://github.com/tplgy/cppcodec.git && cd cppcodec && sudo cp -r cppcodec /usr/include/ && cd .. && sudo rm -rf cppcodec
```

#### **Arch Linux**

```bash
sudo pacman -S nlohmann-json curl poco cppcodec cmake gcc git g++
```

#### **Ubuntu/Debian**

```bash
sudo apt install nlohmann-json3-dev libcurl4-openssl-dev libpoco-dev cmake gcc git g++
```

#### **macOS (Homebrew)**

```bash
brew install nlohmann-json curl poco cppcodec cmake gcc git g++
```

---

## Usage

First clone the repo on your system.

```bash
git clone https://github.com/Deepak22903/fb_daily_post.git && cd fb_daily_post
```

Now build using cmake.

```bash
mkdir build output && cd build && cmake .. && cmake --build . && cd ..
```

This will generate the fb_poster executable file.

---
