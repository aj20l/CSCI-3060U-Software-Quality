# This script is used to create test files for the refund commands. 
# It uses the mk function defined in mk_test.sh to generate the input and expected output files for each test case.
source ./mk_test.sh

mk refund_success <<'EOF'
login
admin01
refund
buyer01
seller01
10
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter buyer's username: Enter seller's username: Enter amount of credit to refund: Refunded $10.00 from seller01 to buyer01.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
05 buyer01         seller01        000010.00
00 admin01         AA 000500.00
EOF

mk refund_fail_1_invalid_users <<'EOF'
login
admin01
refund
buyer123
invalidSeller
100
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter buyer's username: Enter seller's username: Enter amount of credit to refund: Error: The buyer and the seller must both be current users.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk refund_fail_2_invalid_amount <<'EOF'
login
admin01
refund
buyer01
seller01
500
logout
--- terminal
Enter transaction: Enter username: Logged in as admin01.
Enter transaction: Enter buyer's username: Enter seller's username: Enter amount of credit to refund: Error: The seller does not have enough credit for that refund.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 admin01         AA 000500.00
EOF

mk refund_fail_3_not_admin <<'EOF'
login
fulluser
refund
logout
--- terminal
Enter transaction: Enter username: Logged in as fulluser.
Enter transaction: Error: Your account type is not permitted to do that.
Enter transaction: Logged out.
Enter transaction: 
--- transactions
00 fulluser        FS 000100.00
EOF