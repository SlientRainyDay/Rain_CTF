echo $GZCTF_FLAG>/home/ctf/Nep_CTF_FLAG_ONE
nohup /home/ctf/serve >result 2>&1 &
sleep 1
/home/ctf/safebox /home/ctf/client