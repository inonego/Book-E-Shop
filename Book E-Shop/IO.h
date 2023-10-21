#pragma once

#include <any>
#include <vector>
#include <string>
#include <functional>

using namespace std; 

class Parser;

class IO {
public:  
	class Buffer {
	public:
		struct CheckPoint { friend class Buffer; private: int index; public: CheckPoint(int index) : index(index) {} };
	private:
		vector<string> buffer;
	public:
		CheckPoint checkpoint();
		void rollback(CheckPoint checkpoint);
		void clear();
		void write(string text);
		string text();
	}; 

	using CheckPoint = Buffer::CheckPoint;
private:    
	Buffer* buffer = nullptr;
public:
	// ȭ�鿡 ��µ� ��� �ؽ�Ʈ�� ����ϴ�.
	void clear();

	// ȭ�鿡 ��ü �ؽ�Ʈ�� �ٽ� ����մϴ�.
	void reprint();

	// ȭ�鿡 �ؽ�Ʈ�� ����մϴ�.
	virtual void print(string text);

	// ȭ�鿡�� �Է��� �޽��ϴ�.
	virtual string input();

	// üũ����Ʈ�� �����մϴ�.
	CheckPoint checkpoint();

	// ������ üũ����Ʈ�� �ǵ����ϴ�.
	void rollback(CheckPoint checkpoint);

	// �۾��� ���۸� �����մϴ�.
	void set_buffer(Buffer* buffer);
	Buffer* get_buffer();
};


class MenuIO : public IO {
private:
	function<bool(MenuIO&, string)> input_func;
public:
	MenuIO(function<bool(MenuIO&, string)> input_func) : input_func(input_func) {}

	// ȭ�鿡�� �Է��� �޽��ϴ�.
	string input() override;

	// �ʿ��� ������ ����ϰ�, ȭ�鿡�� �Է��� �޽��ϴ�.
	string input(string msg_info, string label = "�Է�");

	// Parser�� �̿��� �Է��� �ް�, �ùٸ� �����̸� �Ľ��Ͽ� ��ȯ�մϴ�.
	any input(Parser* parser);

	// "�ƹ�Ű�� �Է��ϼ���..."�� ����ϰ� ����մϴ�.
	void pause();

	// ȭ�鿡 ���м��� �׸��ϴ�.
	void print_line(bool bold = true, int length = 40);
};