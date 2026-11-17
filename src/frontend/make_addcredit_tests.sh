# This script is used to create test files for the addcredit commands. 
# It uses the mk function defined in mk_test.sh to generate the input and expected output files for each test case.
source ./mk_test.sh

mk addcredit_success <<'EOF'
login
admin01
addcredit
500
fulluser
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter amount of credit to add: Enter username to credit: Added $500.00 credit to fulluser.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
06 fulluser        FS 000500.00
00 admin01         AA 000500.00
EOF

mk addcredit_fail_1_invalid_username <<'EOF'
login
admin01
addcredit
100
nobody
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter amount of credit to add: Enter username to credit: Error: No such user.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk addcredit_fail_2_exceeds_1000 <<'EOF'
login
admin01
addcredit
1500
fulluser
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter amount of credit to add: Enter username to credit: Error: A maximum of $1000.00 can be added to an account per session.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk addcredit_fail_3_cumulative_limit <<'EOF'
login
admin01
addcredit
600
fulluser
addcredit
500
fulluser
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter amount of credit to add: Enter username to credit: Added $600.00 credit to fulluser.
Enter transaction: Enter amount of credit to add: Enter username to credit: Error: A maximum of $1000.00 can be added to an account per session.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
06 fulluser        FS 000600.00
00 admin01         AA 000500.00
EOF

mk addcredit_success_standard_user <<'EOF'
login
buyer01
addcredit
25.50
logout
--- terminal
Enter transaction: Enter username: Logged in as buyer01.
Enter transaction: Enter amount of credit to add: Added $25.50 credit to buyer01.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
06 buyer01         BS 000025.50
00 buyer01         BS 000125.50
EOF