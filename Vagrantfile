Vagrant.configure("2") do |config|
   config.vm.box = "ubuntu/trusty64"

   config.vm.synced_folder '.', '/mnt/host', create: true

   Vagrant.configure("2") do |config|
     config.vm.provision "shell", path: "/mnt/host/vagrant-provision.sh"
   end
end
