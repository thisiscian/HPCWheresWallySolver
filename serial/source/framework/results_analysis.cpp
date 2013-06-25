#include "whereswally/framework/results_analysis.h"

using namespace std;
  struct classcomp {
    bool operator() (const Pattern_Result& lhres, const Pattern_Result& rhres) const {
      return lhres.info.get_confidence()*lhres.certainty>rhres.info.get_confidence()*rhres.certainty;
    }
  };

void Results_Analysis::calculate_final_results(int number_of_final_results, std::vector<Pattern_Result> results) {
  final_results.clear();
  set<Pattern_Result, classcomp> result_out; 
  set<Pattern_Result, classcomp>::iterator it;
  for(int i=0; i<results.size(); i++) {
    result_out.insert(results[i]);
  }

  it=result_out.begin();
  int i=0;
  while(i<number_of_final_results && it != result_out.end()) {
    final_results.push_back(*it);
    ++i;
    ++it;
  }
}

void Results_Analysis::add_to_final_results(std::vector<Pattern_Result> results) {
  for(int i=0; i<results.size(); i++) {
    final_results.push_back(results[i]);
  }
}

vector<Pattern_Result> Results_Analysis::get_final_results() {
  return final_results;
}

