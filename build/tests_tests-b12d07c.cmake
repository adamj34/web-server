add_test( [==[Request parsing]==] /home/addar/web-server/build/tests [==[Request parsing]==]  )
set_tests_properties( [==[Request parsing]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[Request to_string]==] /home/addar/web-server/build/tests [==[Request to_string]==]  )
set_tests_properties( [==[Request to_string]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[Response set_header]==] /home/addar/web-server/build/tests [==[Response set_header]==]  )
set_tests_properties( [==[Response set_header]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[Content-Length header is automatically set]==] /home/addar/web-server/build/tests [==[Content-Length header is automatically set]==]  )
set_tests_properties( [==[Content-Length header is automatically set]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[Response to_string]==] /home/addar/web-server/build/tests [==[Response to_string]==]  )
set_tests_properties( [==[Response to_string]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[Endpoint manager add endpoint]==] /home/addar/web-server/build/tests [==[Endpoint manager add endpoint]==]  )
set_tests_properties( [==[Endpoint manager add endpoint]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[FileHandler send file - Existing file]==] /home/addar/web-server/build/tests [==[FileHandler send file - Existing file]==]  )
set_tests_properties( [==[FileHandler send file - Existing file]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[FileHandler send file - Non-existing file]==] /home/addar/web-server/build/tests [==[FileHandler send file - Non-existing file]==]  )
set_tests_properties( [==[FileHandler send file - Non-existing file]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[FileHandler write file]==] /home/addar/web-server/build/tests [==[FileHandler write file]==]  )
set_tests_properties( [==[FileHandler write file]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
set( tests_TESTS [==[Request parsing]==] [==[Request to_string]==] [==[Response set_header]==] [==[Content-Length header is automatically set]==] [==[Response to_string]==] [==[Endpoint manager add endpoint]==] [==[FileHandler send file - Existing file]==] [==[FileHandler send file - Non-existing file]==] [==[FileHandler write file]==])
