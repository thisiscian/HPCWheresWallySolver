#include "whereswally/framework/results_analysis.h"
using namespace std;

//**
//**  RELATIONAL OPERATOR
//**

bool wwp::compare_results (const Pattern_Result& lhres, const Pattern_Result& rhres) {
  //-- true if results are more confident and more certain
  return lhres.info.get_confidence()*lhres.certainty>rhres.info.get_confidence()*rhres.certainty;
}

//**
//**  GET/SET METHODS
//**

void Results_Analysis::add_to_final_results(std::vector<Pattern_Result> results) {
  for(size_t i=0; i<results.size(); i++) {
    final_results.push_back(results[i]);
  }
}

vector<Pattern_Result> Results_Analysis::get_final_results() {
  return final_results;
}

//**
//** ANALYSE FUNCTION
//**

void Results_Analysis::calculate_final_results(size_t number_of_final_results, std::vector<Pattern_Result> results) {
  final_results.clear();
  //-- order results
  sort(results.begin(), results.end(), wwp::compare_results);
  //-- add desired number of results to final results
  for(size_t i=0; i<number_of_final_results && i<results.size(); i++) {
    //-- if results are less that 1 in scale, remove them and continue
    if(results[i].scale[0] <=0 || results[i].scale[1] <= 0) {results.erase(results.begin()+i); i--; continue;}
    final_results.push_back(results[i]);
  }

}


