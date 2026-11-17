# This script is used to create test files for the sell commands. 
# It uses the mk function defined in mk_test.sh to generate the input and expected output files for each test case.
source ./mk_test.sh

mk sell_success <<'EOF'
login
seller01
sell
Terraria
29.99
logout
--- terminal
Enter transaction: Enter username: Logged in as seller01.
Enter transaction: Enter game name: Enter price in dollars (e.g. 15.00): Terraria is now for sale.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
03 Terraria                   seller01        029.99
00 seller01        SS 000050.00
EOF

mk sell_fail_1_price_exceeds_limit <<'EOF'
login
admin01
sell
Portal
1200.00
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter game name: Enter price in dollars (e.g. 15.00): Error: The price cannot be more than 999.99.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk sell_fail_2_name_too_long <<'EOF'
login
admin01
sell
SuperAwesomeMinecraftGame123
50.00
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter game name: Enter price in dollars (e.g. 15.00): Error: Game names can be at most 25 characters long.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk sell_fail_3_price_not_a_number <<'EOF'
login
admin01
sell
Portal
abc
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter game name: Enter price in dollars (e.g. 15.00): Error: That is not a valid price.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk sell_fail_4_duplicate_name <<'EOF'
login
admin01
sell
Minecraft
50.00
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter game name: Enter price in dollars (e.g. 15.00): Error: A game with that name already exists.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk sell_fail_5_buy_standard_cannot_sell <<'EOF'
login
buyer01
sell
logout
--- terminal
Enter transaction: Enter username: Logged in as buyer01.
Enter transaction: Error: Your account type is not permitted to do that.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 buyer01         BS 000100.00
EOF