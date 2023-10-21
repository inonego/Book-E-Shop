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

string MenuIO::input()
{
	return this->input("");
}

string MenuIO::input(string msg_info, string label)
{
	auto checkpoint = this->checkpoint();

	string input;

	while (true) {
		if (!msg_info.empty()) this->print(msg_info + "\n");

		this->print(label + " : ");

		input = IO::input();

		// 입력을 받아서 검사하는 함수에 집어넣습니다.
		// 만약 검사를 통과하면 루프를 벗어나고,
		// 만약 검사를 통과하지 못하면 다시 입력을 받습니다.
		if (input_func(*this, input)) {
			break;
		}
		else {
			this->rollback(checkpoint);
		}
	}

	return input;
}

any MenuIO::input(Parser* parser)
{
	any result;

	auto checkpoint = this->checkpoint();

	while (true) {
		string input = this->input(parser->msg_info, parser->label);

		if (parser->Check(input)) {
			result = parser->Parse(input);

			break;
		}
		else {
			this->print(parser->msg_error + '\n'); this->pause();

			this->rollback(checkpoint);
		}
	}

	return result;
}

void MenuIO::pause()
{
	this->print("아무키나 입력하세요... \n");

	IO::input();
}

void MenuIO::print_line(bool bold, int length)
{
	for (int i = 0; i < length; i++) {
		this->print(bold ? "=" : "-");
	}

	this->print("\n");
} 