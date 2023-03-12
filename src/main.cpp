#include <chrono>
#include <iostream>
#include <string>
#include <ncurses.h>
#include <random>

void init() {
  initscr(); // создает "экран"
  clear();   // очищает его
  noecho(); // не выводит нажатые клавиши в консоль
  cbreak(); // не ждет enter от пользователя, а сразу считывает клавишу
  curs_set(0);          // прячет курсор
  keypad(stdscr, TRUE); // позволяет считывать доп клавиши типа стрелок
  border(0, 0, 0, 0, 0, 0, 0, 0); // Рисует границы
}

void deinit() {
  endwin(); // деинициализирует окно и стирает все что было
}

// Функция генерирует "слово" - строку со случайными символами
std::string generate_word(int length) {
  const char possible_chars[] = "abcdefghijklmnopqrstuvwxyz";
  std::string result = "";
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 25);
  for (int i = 0; i < length; i++) {
    result += possible_chars[dis(gen)];
  }
  return result;
}

std::string generate_word() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(3, 10);
  int length = dis(gen);
  const char possible_chars[] = "abcdefghijklmnopqrstuvwxyz";
  std::string result = "";
  srand(time(0));
  for (int i = 0; i < length; i++) {
    result += possible_chars[dis(gen)];
  }
  return result;
}

std::string generate_sentence(int words_count){
  std::string result = "";
  for(int i = 0; i<words_count; i++){
    if(i > 0){
      result += " ";
    }
    result += generate_word();
  }
  return result;
}

// Эта функция должна печатать в терминале новое слово (и другую информацию,
// если нужно). typed_number - количество букв слова, которые уже нажаты и
// должны быть отображены звездочками.
void update_screen(const std::string &string, int typed_number) {
  move(1, 1);
  printw("type this word: ");
  for (int i = 0; i < string.length(); i++) {
    if (i >= typed_number) {
      addch(string[i]);
    }else if(string[i] == ' '){
      addch(' ');
    } else {
      addch('*');
    }
  }
}


auto end() { return std::chrono::steady_clock::now(); }

void display_stats(std::chrono::steady_clock::time_point start, int typed_char) {
  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end() - start);
  int speed = typed_char * 60 / (time.count()/1000.0);
  clear();
  printw("Time taken: %ld ms", time.count());
  printw("\n Speed: %d ms per char", speed);
  printw("\n good job!, wanna more? if yes press enter, else esc");
}

void start() {
  std::string string_to_type = generate_sentence(3);
  int typed_number = 0;
  int typed_char = 0;
  update_screen(string_to_type, typed_number);

  auto start_time = std::chrono::steady_clock::now();

  while (true) {
    int key = getch();
    if (key == 27) {
      return;
    }
    if (key == string_to_type[typed_number]) {
      typed_number++;
      typed_char++;
    }
    if (typed_number == string_to_type.length()) {
      display_stats(start_time, typed_char);
      return;
    }
    update_screen(string_to_type, typed_number);
  }
}

int main(int argc, char **argv) {
  init();
  printw("Hello, \n press enter to start");
  int key = getch();
  while (key != 27) {
    clear();
    if (key == '\n') {
      start();
    }
    key = getch();
  }
  deinit();
  return 0;
}