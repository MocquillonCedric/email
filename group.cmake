
function(group_sources sources)
	foreach(f ${ARGV})
		# Get the path of the file relative to ${DIRECTORY},
		# then alter it (not compulsory)
		file(RELATIVE_PATH SRCGR ${CMAKE_SOURCE_DIR} ${f})

		# Extract the folder, ie remove the filename part
		string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})

		# Source_group expects \\ (double antislash), not / (slash)
		string(REPLACE / \\ SRCGR ${SRCGR})
		source_group("${SRCGR}" FILES ${f})
	endforeach()
	
	set(Result ${ARGV} PARENT_SCOPE)
endfunction()