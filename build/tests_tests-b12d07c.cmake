add_test( [==[Request parsing]==] /home/addar/web-server/build/tests [==[Request parsing]==]  )
set_tests_properties( [==[Request parsing]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[Request to_string]==] /home/addar/web-server/build/tests [==[Request to_string]==]  )
set_tests_properties( [==[Request to_string]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[Response set_header]==] /home/addar/web-server/build/tests [==[Response set_header]==]  )
set_tests_properties( [==[Response set_header]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[Response to_string]==] /home/addar/web-server/build/tests [==[Response to_string]==]  )
set_tests_properties( [==[Response to_string]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
add_test( [==[Endpoint manager add endpoint]==] /home/addar/web-server/build/tests [==[Endpoint manager add endpoint]==]  )
set_tests_properties( [==[Endpoint manager add endpoint]==] PROPERTIES WORKING_DIRECTORY /home/addar/web-server/build)
set( tests_TESTS [==[Request parsing]==] [==[Request to_string]==] [==[Response set_header]==] [==[Response to_string]==] [==[Endpoint manager add endpoint]==])
