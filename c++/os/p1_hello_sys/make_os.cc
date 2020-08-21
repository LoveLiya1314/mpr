#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>

class HelloSys {

public:
	explicit HelloSys(const std::string &s) : show_text_(s){	
	}
	~HelloSys() {}
	void load_boot_file() {
		base_start_code_.clear();
		std::ifstream f;		
		f.open("./boot", std::ios::binary);
		while(f.good()) {
			char buf = 0;
			if(f.read(&buf, 1)) {
				base_start_code_.push_back(buf);	
			}
		}
		f.close();
	}
	void make_floppy() {
		write_data_ = base_start_code_;
		write_data_.push_back(0x0a);
		write_data_.push_back(0x0a);
		// 添加打印字符
		for (auto c : show_text_)
			write_data_.push_back(c);
		
		write_data_.push_back(0x0a);
		// 0x1fe == 512
		auto data_size = write_data_.size();
		for(auto i = 0; i < (0x1fe - data_size); ++i) {
			write_data_.push_back(0);
		}

		write_data_.push_back(0x55);
		write_data_.push_back(0xaa);
		write_data_.push_back(0xf0);
		write_data_.push_back(0xff);
		write_data_.push_back(0xff);
		data_size = write_data_.size();
		for(auto i = 0; i < (0x1000 - data_size); ++i) {
			write_data_.push_back(0);
		}
		std::ofstream of;
		of.open("os.img");
		std::cout << std::hex <<  "write size: " << write_data_.size() << std::endl;
		for(auto c : write_data_) {
			of << c;
		}
		of.close();
	}

private:
	std::string show_text_;
	std::vector<unsigned char> write_data_;
        std::vector<unsigned char> base_start_code_ {
	0xeb,0x4e,0x90,0x48,0x45,0x4c,0x4c,0x4f,0x49,0x50,0x4c,0x00,0x02,0x01,0x01,0x00,
	0x02,0xe0,0x00,0x40,0x0b,0xf0,0x09,0x00,0x12,0x00,0x02,0x00,0x00,0x00,0x00,0x00,
	0x40,0x0b,0x00,0x00,0x00,0x00,0x29,0xff,0xff,0xff,0xff,0x48,0x45,0x4c,0x4c,0x4f,
	0x2d,0x4f,0x53,0x20,0x20,0x20,0x46,0x41,0x54,0x31,0x32,0x20,0x20,0x20,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xb8,0x00,0x00,0x8e,0xd0,0xbc,0x00,0x7c,0x8e,0xd8,0x8e,0xc0,0xbe,0x74,0x7c,0x8a,
    0x04,0x83,0xc6,0x01,0x3c,0x00,0x74,0x09,0xb4,0x0e,0xbb,0x0f,0x00,0xcd,0x10,0xeb,
        0xee,0xf4,0xeb,0xfd };
};

int main() {
	HelloSys os("Hello OS!");
        //os.load_boot_file();
	os.make_floppy();
}
