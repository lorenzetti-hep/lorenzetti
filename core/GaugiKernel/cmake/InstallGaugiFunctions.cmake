

function(gaugi_initialize)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/python)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/scripts)
endfunction(gaugi_initialize)


function(gaugi_install_python_modules filepath module)
  message(STATUS "${filepath} -> ${CMAKE_BINARY_DIR}/python/${module}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filepath} ${CMAKE_BINARY_DIR}/python/${module})
endfunction(gaugi_install_python_modules)


function(gaugi_install_scripts filepath)

  file(GLOB SCRIPTS ${filepath}/*)
  FOREACH( SCRIPT ${SCRIPTS})
    get_filename_component(sname ${SCRIPT} NAME)
    message(STATUS "${SCRIPT} -> ${CMAKE_BINARY_DIR}/scripts/${sname}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${SCRIPT} ${CMAKE_BINARY_DIR}/scripts/${sname})
  ENDFOREACH()

endfunction(gaugi_install_scripts)




function( gaugi_finalize )

  file(GLOB SO_FILES ${CMAKE_BINARY_DIR}/*.so)
  FOREACH( SFILE ${SO_FILES})
    execute_process(POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${SFILE} ${CMAKE_BINARY_DIR}/lib/)
  ENDFOREACH()

endfunction( gaugi_finalize )
