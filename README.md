# JAVM - Just Another Virtual Machine
Mô tả của bài tập lớn nằm trong file JAVM_Specifications_Vietnamese.pdf

Cách chạy code trên terminal:
+ Bước 1: g++ -o main main.cpp StackFrame.cpp
+ Bước 2: ./main

Các lệnh được thêm vào so với đặc tả pdf:
+ exit: Thoát khỏi JAVM
+ reset: Trả trạng thái JAVM về trạng thái ban đầu
+ switch <val>: chuyển sang page val (ví dụ: switch 1)
+ new: tạo thêm page mới
+ clear: xóa toàn bộ pages
+ rm <val>: xóa page val (ví dụ: rm 1)
+ clscr: xóa màn hình terminal của JAVM
