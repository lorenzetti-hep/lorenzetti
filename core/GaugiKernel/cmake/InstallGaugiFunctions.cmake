

function(gaugi_initialize)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/python)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/scripts)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)
endfunction(gaugi_initialize)


function(gaugi_install_python_modules filepath module)
  message(STATUS "${filepath} -> ${CMAKE_BINARY_DIR}/python/${module}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filepath} ${CMAKE_BINARY_DIR}/python/${module})
endfunction(gaugi_install_python_modules)


function(gaugi_install_scripts_modules filepath module)
  message(STATUS "${filepath} -> ${CMAKE_BINARY_DIR}/scripts/${module}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filepath} ${CMAKE_BINARY_DIR}/scripts/${module})
endfunction(gaugi_install_scripts_modules)


function(gaugi_install_data_modules filepath module)
  message(STATUS "${filepath} -> ${CMAKE_BINARY_DIR}/data/${module}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filepath} ${CMAKE_BINARY_DIR}/data/${module})
endfunction(gaugi_install_data_modules)


function( gaugi_finalize )
  file(GLOB SO_FILES ${CMAKE_BINARY_DIR}/*.so)
  FOREACH( SFILE ${SO_FILES})
    execute_process(POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${SFILE} ${CMAKE_BINARY_DIR}/lib/)
  ENDFOREACH()
endfunction( gaugi_finalize )
