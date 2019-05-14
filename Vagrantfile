# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
    config.vm.define "hyu_os_2019", primary: true do |os|
        os.vm.box = "ubuntu/trusty64"
        os.vm.network "public_network"
        os.vm.synced_folder ".", "/vagrant"

        os.vm.provider "virtualbox" do |vb|
            vb.gui = false
            vb.memory = "2048"
        end
    end
end
