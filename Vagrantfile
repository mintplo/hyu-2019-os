# -*- mode: ruby -*-
# vi: set ft=ruby :

# Ubuntu 16.04 LTS (xenial64)

Vagrant.configure(2) do |config|

  config.vm.define "hyu_os_2019", primary: true do |web|
    web.vm.box = "ubuntu/xenial64"
    web.vm.network "private_network", ip: "192.168.50.100"

    # NFS mount for fix permission issue
    web.vm.synced_folder ".", "/vagrant", :nfs => false

    web.vm.provider :virtualbox do |vb|
      vb.name = "hyu_os_2019"
      vb.gui = false
      vb.memory = "1024"
    end

    web.vm.post_up_message = "hyu os 2019 provisioning completed!"
  end

end

