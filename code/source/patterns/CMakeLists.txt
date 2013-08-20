set(source_list "pattern_tools" "redandwhite" "bluetrousers" "findglasses"
"findfeatures")

foreach(source ${source_list})
	get_filename_component(${source} ${source}.cpp ABSOLUTE)
	set_property(GLOBAL APPEND PROPERTY SOURCE ${${source}})
endforeach(source)

