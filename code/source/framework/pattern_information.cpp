#include "whereswally/framework/pattern_information.h"
using namespace std;

//**
//**  CLASS INITIALISATION
//**

Pattern_Information::Pattern_Information() {
  name = "";
  description = "";
  confidence = 1.0;
}

//**
//**  RELATIONAL OPERATORS
//**

const bool Pattern_Information::operator==(const Pattern_Information &rhs) {
  bool check = true;
  check &= (get_name() == rhs.get_name());
  check &= (get_description() == rhs.get_description());
  check &= (get_confidence() == rhs.get_confidence());
  return check;
}

const bool Pattern_Information::operator!=(const Pattern_Information &rhs) {
  return !(*this == rhs);
}

//**
//**  GET/SET METHODS
//**

void Pattern_Information::set_name(std::string new_name) {
  name = new_name;
}

string Pattern_Information::get_name() const {
  return name;
}

void Pattern_Information::set_description(std::string new_description) {
  description = new_description;
}

string Pattern_Information::get_description() const {
  return description;
}

void Pattern_Information::set_confidence(float new_confidence) {
  if(new_confidence >=0) {
    confidence = new_confidence;
  } else {
    cerr << "Error: Confidence values should be non-negative" << endl;
    throw NEGATIVE_CONFIDENCE;
  }
}

float Pattern_Information::get_confidence() const {
  return confidence;
}

