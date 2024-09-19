#include "functions.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

std::string StringToLower(const std::string& word) {
  std::string result = word;

  for (char& c : result) {
    c = tolower(c);  // Convert each character to lowercase
  }
  return result;
}

bool IsVowelGroup(const char& v1, const char& v2) {
  return (v1 == 'a' && v2 == 'i') || (v1 == 'a' && v2 == 'e') ||
         (v1 == 'a' && v2 == 'o') || (v1 == 'a' && v2 == 'u') ||
         (v1 == 'e' && v2 == 'i') || (v1 == 'e' && v2 == 'u') ||
         (v1 == 'i' && v2 == 'u') || (v1 == 'o' && v2 == 'i') ||
         (v1 == 'o' && v2 == 'u') || (v1 == 'u' && v2 == 'i');
}

/* bool IsPunctionGroup(const char& v1, const char& v2) {
  if (v1 == '-' && v2 == '\'') {
    return true;
  }
  return false;
} */

bool IsVowel(const char& v) {
  return (v == 'a' || v == 'e' || v == 'i' || v == 'o' || v == 'u');
}

bool IsConsonant(const char& c) {
  return (c == 'p' || c == 'k' || c == 'h' || c == 'l' || c == 'm' ||
          c == 'n' || c == 'w');
}

bool IsSpecialCharacter(const char& c) { return (c == ' ' || c == '\''); }

bool IsValidCharacter(const char& c) {
  return (IsVowel(c) || IsConsonant(c) || IsSpecialCharacter(c));
}

bool IsValidWord(const std::string& word) {
  for (char c : word) {
    if (!IsValidCharacter(c)) {
      return false;
    }
  }
  return true;
}

char ConsonantPronunciation(const char& c, const char& prev) {
  if (c == 'w' && (prev == 'a' || prev == 'u' || prev == 'o')) {
    return 'w';
  }

  if (c == 'w' && (prev == 'i' || prev == 'e')) {
    return 'v';
  }

  // Returning c will allow random letters to be returned as well
  return c;
}

std::string VowelGroupPronunciation(const char& v1, const char& v2) {
  std::string correct;
  if ((v1 == 'a' && v2 == 'i') || (v1 == 'a' && v2 == 'e')) {
    correct = "eye";
  } else if ((v1 == 'a' && v2 == 'o') || (v1 == 'a' && v2 == 'u') ||
             (v1 == 'o' && v2 == 'u')) {
    correct = "ow";
  } else if ((v1 == 'e' && v2 == 'i')) {
    correct = "ay";
  } else if ((v1 == 'i' && v2 == 'u')) {
    correct = "ew";
  } else if ((v1 == 'o' && v2 == 'i')) {
    correct = "oy";
  } else if ((v1 == 'u' && v2 == 'i')) {
    correct = "ooey";
  } else if (v1 == 'e' && v2 == 'u') {
    correct = "eh-oo";
  }
  return correct;
}

std::string SingleVowelPronunciation(const char& v) {
  std::string pronunciation;
  if (v == 'a') {
    pronunciation = "ah";
  } else if (v == 'e') {
    pronunciation = "eh";
  } else if (v == 'i') {
    pronunciation = "ee";
  } else if (v == 'o') {
    pronunciation = "oh";
  } else if (v == 'u') {
    pronunciation = "oo";
  } else {
    // If the input is not a vowel, return the original character
    pronunciation = v;
  }
  return pronunciation;
}

std::string ProcessCharacter(const char& prev,
                             const char& curr,
                             const char& next) {
  std::string word;

  if (IsVowel(curr)) {
    if (IsVowelGroup(curr, next)) {
      if (curr == 'u' && IsVowelGroup(prev, curr)) {
        if (next == 'i') {
          word = "-";
        } else {
          word = "ee";
        }
      } else if (curr == 'u' && next == 'i') {
        word = "ee";
      } else {
        word = VowelGroupPronunciation(curr, next);
      }
    } else {
      if (curr == 'i' && (prev == 'a' || prev == 'e')) {
        word = "";
      } else {
        word = SingleVowelPronunciation(curr);
      }
    }
  }

  if (curr == '\'') {
    word = "'";
  } else if (curr == ' ') {
    word = " ";
  }
  if (IsConsonant(curr)) {
    word = ConsonantPronunciation(curr, prev);
  }

  return word;
}

std::string Pronunciation(const std::string& word) {
  if (IsValidWord(word)) {
    std::string result;  // Variable to store the pronunciation
    for (size_t i = 0; i < word.size(); ++i) {
      char prev = '\0';
      if (i > 0) {
        prev = word[i - 1];
      } else {
        prev = '\0';  // Null character for the first character
      }
      char curr = word[i];  // Current character
      char next = '\0';
      if (i < word.size() - 1) {
        next = word[i + 1];
      } else {
        next = '\0';
      }
      result += ProcessCharacter(prev, curr, next);
      if (IsVowel(curr) && i < word.size() - 1) {
        if (IsVowelGroup(curr, next) || next == ' ' || next == '\'') {
          continue;
        }
        result += "-";
      }
      if (!(prev == 'h')) {
        if (curr == 'o' && next == 'a') {
          result += "-";
        }
      }
    }  // Return the final pronunciation
  }
  return "need bro";
}

std::string GetPronunciation(const std::string& hawaiian_word) {
  std::string need = StringToLower(hawaiian_word);
  if ((IsValidWord(need))) {
    return Pronunciation(need);
  }
  throw std::invalid_argument("Not Valid");
}
