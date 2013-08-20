#include "whereswally/framework/search_pattern.h"
using namespace std;
using namespace cv;

//**
//**  RELATIONAL OPERATORS
//**

//-- entire pattern result must be equal
const bool operator==(const Pattern_Result &lhs, const Pattern_Result &rhs) {
  bool check = true;
  Pattern_Information li = lhs.info;
  Pattern_Information ri = rhs.info;

  check &= (li == ri);
  check &= (lhs.wally_location[0] == rhs.wally_location[0]);
  check &= (lhs.wally_location[1] == rhs.wally_location[1]);
  check &= (lhs.scale[0] == rhs.scale[0]);
  check &= (lhs.scale[1] == rhs.scale[1]);
  check &= (lhs.certainty == rhs.certainty);
  return check;
}

const bool operator!=(const Pattern_Result &lhs, const Pattern_Result &rhs) {
  return !(lhs == rhs);
}

//-- pattern must be equal except for the certainty
const bool operator>(const Pattern_Result &lhs, const Pattern_Result &rhs) {
  bool check = true;
  Pattern_Information li = lhs.info;
  Pattern_Information ri = rhs.info;

  check &= (li == ri);
  check &= (lhs.wally_location[0] == rhs.wally_location[0]);
  check &= (lhs.wally_location[1] == rhs.wally_location[1]);
  check &= (lhs.scale[0] == rhs.scale[0]);
  check &= (lhs.scale[1] == rhs.scale[1]);
  check &= (lhs.certainty > rhs.certainty);
  return check;
}
//**
//**  GET/SET METHODS
//**

const Pattern_Information Search_Pattern::get_pattern_information() {
  return info;
}

vector<Pattern_Result> Search_Pattern::start_search(Mat image) {
  vector<Pattern_Result> placeholder;
  return placeholder;
}

