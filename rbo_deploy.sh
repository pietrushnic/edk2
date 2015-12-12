build -a X64 -p ShellPkg/ShellPkg.dsc -t GCC49 -n$(nproc)

if [ ! -f app.disk  ]; then
  echo "ERROR: app.disk not found!"
  exit
fi

if [ ! -d mnt_app ]; then
  mkdir -p mnt_app
fi

sudo mount app.disk mnt_app
sudo cp Build/Shell/DEBUG_GCC49/X64/ShellPkg/Application/ShellRmAllBootOptApp/RmAllBootOptApp/OUTPUT/RmAllBootOpts.efi mnt_app
sudo umount mnt_app
./ovmf.sh

