# This script is used to create test files for the create commands. 
# It uses the mk function defined in mk_test.sh to generate the input and expected output files for each test case.
source ./mk_test.sh

mk create_success <<'EOF'
login
admin01
create
newplayer
full-standard
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter new username: Enter account type (admin, full-standard, buy-standard, sell-standard): User newplayer created.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
01 newplayer       FS 000000.00
00 admin01         AA 000500.00
EOF

mk create_fail_1_user_exists <<'EOF'
login
admin01
create
buyer01
admin
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter new username: Enter account type (admin, full-standard, buy-standard, sell-standard): Error: That username is already taken.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk create_fail_2_username_too_long <<'EOF'
login
admin01
create
abcdefghijklmnop
admin
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter new username: Enter account type (admin, full-standard, buy-standard, sell-standard): Error: Usernames can be at most 15 characters long.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk create_fail_3_non_admin <<'EOF'
login
fulluser
create
logout
--- terminal
Enter transaction: Enter username: Logged in as fulluser.
Enter transaction: Error: Your account type is not permitted to do that.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 fulluser        FS 000100.00
EOF

mk create_fail_4_invalid_account_type <<'EOF'
login
admin01
create
newplayer
vip
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter new username: Enter account type (admin, full-standard, buy-standard, sell-standard): Error: Unknown account type.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF