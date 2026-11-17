# This script is used to create test files for the logout commands. 
# It uses the mk function defined in mk_test.sh to generate the input and expected output files for each test case.
source ./mk_test.sh

mk logout_success <<'EOF'
login
fulluser
logout
--- terminal
Enter transaction: Enter username: Logged in as fulluser.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 fulluser        FS 000100.00
EOF

mk logout_fail_1_not_logged_in <<'EOF'
logout
--- terminal
Enter transaction: Error: You must log in first.
Enter transaction: 
EOF