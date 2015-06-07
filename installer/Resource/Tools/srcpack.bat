@echo off
cd /D %2 && %1 clean && %1 %3 && %1 release