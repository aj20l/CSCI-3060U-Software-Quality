# This script is used to create test files for the login commands. 
# It uses the mk function defined in mk_test.sh to generate the input and expected output files for each test case.
source ./mk_test.sh

mk login_success <<'EOF'
login
admin01
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk login_fail_1_unknown_username <<'EOF'
login
nobody
--- terminal
Enter transaction: Enter username: Error: Unknown username.
Enter transaction: 
EOF

mk login_fail_2_already_logged_in <<'EOF'
login
admin01
login
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Error: You are already logged in.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF