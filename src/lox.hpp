#ifndef LOX_HPP
#define LOX_HPP

class Lox {
private:
  bool m_had_error{};

public:
  explicit Lox() = default;

  int run_file(char const *script_path);
  int run_prompt();
  void run(char const *source);
};

#endif // LOX_HPP
