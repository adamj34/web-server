#!/bin/bash

req_num=4
printf 'http://localhost:4221/\n%.0s' $(seq 1 $req_num) | xargs -n1 -P ${req_num} curl -s -w '%{http_code}\n' -o /dev/null
