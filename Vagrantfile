Vagrant.configure("2") do |config|
   config.vm.box = "ubuntu/trusty64"

   config.vm.synced_folder '.', '/mnt/host', create: true

   config.vm.provision "shell", path: "vagrant-provision.sh"
end
