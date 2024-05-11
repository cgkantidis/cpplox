#include "parser.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <expr.hpp>
#include <functional>
#include <scanner.hpp>

static constexpr std::vector<std::string>
tokens_to_strings(std::vector<Token> const &tokens) {
  std::vector<std::string> str_tokens;
  str_tokens.reserve(tokens.size());
  std::transform(
      tokens.begin(),
      tokens.end(),
      std::back_inserter(str_tokens),
      std::mem_fn(&Token::literal_to_string));
  return str_tokens;
}

TEST_CASE("Scan number", "[scanner]") {
  Scanner scanner("1234\n");
  std::vector<Token> const tokens = scanner.scan_tokens();
  REQUIRE(!scanner.had_error());
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].literal_to_string() == "1234");
  REQUIRE(tokens[1].literal_to_string() == "EOF");

  std::for_each(tokens.begin(), tokens.end(), std::mem_fn(&Token::free_token));
}

TEST_CASE("Scan number no new line", "[scanner]") {
  Scanner scanner("1234");
  std::vector<Token> const tokens = scanner.scan_tokens();
  REQUIRE(!scanner.had_error());
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].literal_to_string() == "1234");
  REQUIRE(tokens[1].literal_to_string() == "EOF");

  std::for_each(tokens.begin(), tokens.end(), std::mem_fn(&Token::free_token));
}

TEST_CASE("Unexpected characters are skipped", "[scanner]") {
  Scanner scanner(R"(@
#
$^)");
  std::vector<Token> const tokens = scanner.scan_tokens();
  REQUIRE(scanner.had_error());
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].literal_to_string() == "EOF");

  std::for_each(tokens.begin(), tokens.end(), std::mem_fn(&Token::free_token));
}

TEST_CASE("Comments", "[scanner]") {
  Scanner scanner(R"(
// whole line comment
1234 // trailing comment
)");
  std::vector<Token> const tokens = scanner.scan_tokens();
  REQUIRE(!scanner.had_error());
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].literal_to_string() == "1234");
  REQUIRE(tokens[1].literal_to_string() == "EOF");

  std::for_each(tokens.begin(), tokens.end(), std::mem_fn(&Token::free_token));
}

TEST_CASE("Operators", "[scanner]") {
  Scanner scanner(R"(
!*+-/<><=>===!=
! * + - / < > <= >= == !=
)");
  std::vector<Token> const tokens = scanner.scan_tokens();
  REQUIRE(!scanner.had_error());
  REQUIRE(tokens.size() == 23);

  auto const str_tokens = tokens_to_strings(tokens);
  REQUIRE_THAT(
      str_tokens,
      Catch::Matchers::Equals(std::vector<std::string>(
          {"!", "*", "+", "-", "/", "<", ">",  "<=", ">=", "==", "!=", "!",
           "*", "+", "-", "/", "<", ">", "<=", ">=", "==", "!=", "EOF"})));

  std::for_each(tokens.begin(), tokens.end(), std::mem_fn(&Token::free_token));
}

TEST_CASE("Strings", "[scanner]") {
  Scanner scanner(R"(
"this is a string"
"" // empty string
"

" // multi-line empty string
"this is a multi
line string"
)");
  std::vector<Token> const tokens = scanner.scan_tokens();
  REQUIRE(!scanner.had_error());
  REQUIRE(tokens.size() == 5);

  auto const str_tokens = tokens_to_strings(tokens);
  REQUIRE_THAT(
      str_tokens,
      Catch::Matchers::Equals(std::vector<std::string>(
          {"this is a string",
           "",
           "\n\n",
           "this is a multi\nline string",
           "EOF"})));

  std::for_each(tokens.begin(), tokens.end(), std::mem_fn(&Token::free_token));
}

TEST_CASE("Unterminated string", "[scanner]") {
  Scanner scanner(R"("this is an unterminated string)");
  std::vector<Token> const tokens = scanner.scan_tokens();
  REQUIRE(scanner.had_error());
  REQUIRE(tokens.size() == 1);

  auto const str_tokens = tokens_to_strings(tokens);
  REQUIRE_THAT(
      str_tokens,
      Catch::Matchers::Equals(std::vector<std::string>({"EOF"})));

  std::for_each(tokens.begin(), tokens.end(), std::mem_fn(&Token::free_token));
}

TEST_CASE("Numbers", "[scanner]") {
  Scanner scanner(R"(
123
123.000
123.456
)");
  std::vector<Token> const tokens = scanner.scan_tokens();
  REQUIRE(!scanner.had_error());
  REQUIRE(tokens.size() == 4);

  auto const str_tokens = tokens_to_strings(tokens);
  REQUIRE_THAT(
      str_tokens,
      Catch::Matchers::Equals(
          std::vector<std::string>({"123", "123", "123.456", "EOF"})));

  std::for_each(tokens.begin(), tokens.end(), std::mem_fn(&Token::free_token));
}

TEST_CASE("Identifiers", "[scanner]") {
  Scanner scanner(R"(
// reserved words
and
class
else
false
for
fun
if
nil
or
print
return
super
this
true
var
while
// identifiers
AND
And
anD
CLASS
classs
CLASSS
)");
  std::vector<Token> const tokens = scanner.scan_tokens();
  REQUIRE(!scanner.had_error());
  REQUIRE(tokens.size() == 23);

  auto const str_tokens = tokens_to_strings(tokens);
  REQUIRE_THAT(
      str_tokens,
      Catch::Matchers::Equals(std::vector<std::string>(
          {"and",  "class", "else",   "false",  "for",    "fun",
           "if",   "nil",   "or",     "print",  "return", "super",
           "this", "true",  "var",    "while",  "AND",    "And",
           "anD",  "CLASS", "classs", "CLASSS", "EOF"})));

  std::for_each(tokens.begin(), tokens.end(), std::mem_fn(&Token::free_token));
}

TEST_CASE("Pretty printer", "[printer]") {
  // -123 * (45.67) * "asd"
  auto const expr = std::unique_ptr<Expr>(new Binary(
      new Binary(
          new Unary(
              Token(TokenType::MINUS, "-", nullptr, 1),
              new NumericLiteral(123.0)),
          Token(TokenType::STAR, "*", nullptr, 1),
          new Grouping(new NumericLiteral(45.67))),
      Token(TokenType::STAR, "*", nullptr, 1),
      new StringLiteral("asd")));
  fmt::println("{}", expr->to_string());
  REQUIRE(expr->to_string() == "(* (* (- 123) (group 45.67)) \"asd\")");
}

TEST_CASE("Parser", "[parser]") {
  static constexpr auto source = R"src(!!(-123 * (45.67) * "asd") == ("abc" != 42.42))src";
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();
  REQUIRE(!scanner.had_error());

  Parser parser(tokens);
  auto expr = parser.parse();
  fmt::println("{}", expr->to_string());
  REQUIRE(expr);
  REQUIRE(expr->to_string() == R"dst((== (! (! (group (* (* (- 123) (group 45.67)) "asd")))) (group (!= "abc" 42.42))))dst");
}
