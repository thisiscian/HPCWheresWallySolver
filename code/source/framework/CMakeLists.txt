set(source_list "search_pattern" "pattern_information" "results_analysis")

foreach(source ${source_list})
	get_filename_component(${source} ${source}.cpp ABSOLUTE)
	set_property(GLOBAL APPEND PROPERTY SOURCE ${${source}})
endforeach(source)

