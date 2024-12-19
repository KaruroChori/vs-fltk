#!/bin/sh
cc -shared  -rdynamic -fPIC -o libapp.so app.cpp  -lstdc++