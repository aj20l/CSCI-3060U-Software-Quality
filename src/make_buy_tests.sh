# This script is used to create test files for the buy commands. 
# It uses the mk function defined in mk_test.sh to generate the input and expected output files for each test case.
source ./mk_test.sh

mk buy_success <<'EOF'
login
fulluser
buy
Spiderman
seller01
logout
--- terminal
Enter transaction: Enter username: Logged in as fulluser.
Enter transaction: Enter game name: Enter seller's username: Bought Spiderman for $15.00 from seller01.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
04 Spiderman                  seller01        fulluser        015.00
00 fulluser        FS 000085.00
EOF

mk buy_fail_1_sell_standard_cannot_buy <<'EOF'
login
seller01
buy
logout
--- terminal
Enter transaction: Enter username: Logged in as seller01.
Enter transaction: Error: Your account type is not permitted to do that.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 seller01        SS 000050.00
EOF

mk buy_fail_2_game_does_not_exist <<'EOF'
login
admin01
buy
GTA VII
seller01
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter game name: Enter seller's username: Error: That game is not for sale.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk buy_fail_3_not_enough_credit <<'EOF'
login
pooruser
buy
GTA V
deleteme
logout
--- terminal
Enter transaction: Enter username: Logged in as pooruser.
Enter transaction: Enter game name: Enter seller's username: Error: You do not have enough credit to buy that game.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 pooruser        BS 000010.00
EOF

mk buy_fail_4_already_owns_game <<'EOF'
login
fulluser
buy
Celeste
admin01
logout
--- terminal
Enter transaction: Enter username: Logged in as fulluser.
Enter transaction: Enter game name: Enter seller's username: Error: You already own that game.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 fulluser        FS 000100.00
EOF

mk buy_fail_5_listed_this_session <<'EOF'
login
admin01
sell
Portal
9.99
buy
Portal
admin01
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter game name: Enter price in dollars (e.g. 15.00): Portal is now for sale.
Enter transaction: Enter game name: Enter seller's username: Error: That game was listed in this session and cannot be bought until the next session.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
03 Portal                     admin01         009.99
00 admin01         AA 000500.00
EOF