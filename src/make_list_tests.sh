# This script is used to create test files for the list commands. 
# It uses the mk function defined in mk_test.sh to generate the input and expected output files for each test case.
source ./mk_test.sh

mk listaccounts_success <<'EOF'
login
admin01
listaccounts
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Active accounts:
Username        Type           Credit
admin01         admin          500.00
fulluser        full-standard  100.00
buyer01         buy-standard   100.00
seller01        sell-standard  50.00
pooruser        buy-standard   10.00
deleteme        full-standard  20.00
sysadmin        admin          900.00
7 account(s).
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk listaccounts_fail_not_admin <<'EOF'
login
buyer01
listaccounts
logout
--- terminal
Enter transaction: Enter username: Logged in as buyer01.
Enter transaction: Error: Your account type is not permitted to do that.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 buyer01         BS 000100.00
EOF

mk listgames_success <<'EOF'
login
fulluser
listgames
logout
--- terminal
Enter transaction: Enter username: Logged in as fulluser.
Enter transaction: Available games:
Game                       Seller          Price
Spiderman                  seller01        15.00
Hades                      seller01        24.99
Celeste                    admin01         19.99
Minecraft                  fulluser        20.00
GTA V                      deleteme        60.00
5 game(s).
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 fulluser        FS 000100.00
EOF

mk listcollection_success <<'EOF'
login
fulluser
listcollections
logout
--- terminal
Enter transaction: Enter username: Logged in as fulluser.
Enter transaction: Game collection of fulluser:
  Celeste
1 game(s).
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 fulluser        FS 000100.00
EOF