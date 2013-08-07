set(source_list "error_message" "io_variables" "input" "output" "io_control")

foreach(source ${source_list})
	get_filename_component(${source} ${source}.cpp ABSOLUTE)
	set_property(GLOBAL APPEND PROPERTY SOURCE ${${source}})
endforeach(source)

