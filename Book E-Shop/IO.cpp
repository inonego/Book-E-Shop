#include "IO.h"
#include "Util.h"
#include "Parser.h" 

#include <iostream> 

// ################### IO::Buffer ################### 

IO::CheckPoint IO::Buffer::checkpoint()
{
	return { (int)buffer.size() };
}

void IO::Buffer::rollback(IO::CheckPoint checkpoint)
{
	while (checkpoint.index < (int)buffer.size()) buffer.pop_back();
}

void IO::Buffer::clear()
{ 
	buffer.clear();
}

void IO::Buffer::write(string text)
{ 
	buffer.push_back(text);
}

string IO::Buffer::text()
{
	return join(buffer);
}

// ################### IO ################### 

void IO::clear()
{
	system("cls");
}

void IO::reprint()
{ 
	this->clear();

	cout << buffer->text();
}

void IO::print(string text)
{
	buffer->write(text); 

	cout << text;
}

string IO::input()
{
	string input;

	getline(cin, input);

	buffer->write(input + '\n');

	return input;
}

IO::CheckPoint IO::checkpoint()
{
	return buffer->checkpoint();
}

void IO::rollback(IO::CheckPoint checkpoint)
{
	buffer->rollback(checkpoint);

	this->reprint();
}

void IO::set_buffer(Buffer* buffer)
{ 
	if ((this->buffer = buffer) != nullptr) {
		this->reprint();
	}
}

IO::Buffer* IO::get_buffer()
{
	return this->buffer;
}

// ################### MenuIO ###################

void MenuIO::print_aligned_center(string text)
{
	this->print(format("{:^{}}\n", text, width));
}

string MenuIO::input()
{
	return this->input("");
}

string MenuIO::input(string msg_info, string label)
{
	auto checkpoint = this->checkpoint();

	string input;

	while (true) {
		if (!msg_info.empty()) this->print_aligned_center(msg_info);

		this->print(label + " : ");

		input = IO::input();

		// �Է��� �޾Ƽ� �˻��ϴ� �Լ��� ����ֽ��ϴ�.
		// ���� �˻縦 ����ϸ� ������ �����,
		// ���� �˻縦 ������� ���ϸ� �ٽ� �Է��� �޽��ϴ�.
		if (input_func(*this, input)) {
			break;
		}
		else {
			this->rollback(checkpoint);
		}
	}

	return input;
}

void MenuIO::pause()
{
	this->print("�ƹ�Ű�� �Է��ϼ���... \n");

	IO::input();
}

void MenuIO::print_line(bool bold)
{
	for (int i = 0; i < width; i++) {
		this->print(bold ? "=" : "-");
	}

	this->print("\n");
} 