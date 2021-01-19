# -*- mode: ruby -*-
# vi: set ft=ruby :

# Vagrantfile API/syntax version. Don't touch unless you know what you're doing!
VAGRANTFILE_API_VERSION = "2"

# choose how many machines the cluster will contain
N_VMS = 3

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|

  if Vagrant.has_plugin?('vagrant-libvirt')
    config.vm.box = "bunchc/bionic64" # with libvirt
  else
    config.vm.box = "hashicorp/bionic64" 
  end 

  # use a minimal amount of RAM for each node to avoid overwhelming the host
  config.vm.provider "virtualbox" do |v|
    v.memory = 512
    v.cpus = 2
  end
  config.vm.provider "libvirt" do |v|
    v.memory = 512
    v.cpus = 2
  end
  config.vm.network "private_network", type: "dhcp"

  hosts_file = []
  1.upto(N_VMS) do |i|
    config.vm.define vm_name = "node#{i}" do |config|
      config.vm.hostname = vm_name
      ip = "192.168.0.#{100+i}"
      config.vm.network "private_network",
        ip: ip,
        virtualbox__intnet: "clusternet"
    end
  end

  script = <<-SCRIPT
    set -x
    if [[ ! -e /etc/.provisioned ]]; then
      rm /etc/hosts
      for i in {1..N_VMS}; do
        ip="192.168.0.$((100+i))"
        echo "$ip node$i" >> /etc/hosts
      done

      # we only generate the key on one of the nodes
      if [[ ! -e /vagrant/id_rsa ]]; then
        ssh-keygen -t rsa -f /vagrant/id_rsa -N ""
      fi
      install -m 600 -o vagrant -g vagrant /vagrant/id_rsa /home/vagrant/.ssh/
      #install -m 600 -o vagrant -g vagrant /vagrant/authorized_keys /home/vagrant/.ssh/
      # the extra 'echo' is needed because Vagrant inserts its own key without a
      # newline at the end
      (echo; cat /vagrant/id_rsa.pub) >> /home/vagrant/.ssh/authorized_keys

      echo 'StrictHostKeyChecking no' >> /etc/ssh/ssh_config
      apt-get -y update
      apt-get -y install make openmpi-bin libopenmpi-dev
      touch /etc/.provisioned
    fi
  SCRIPT
  script.sub! 'N_VMS', N_VMS.to_s
  config.vm.provision "shell", inline: script
  #config.vm.provision "file", source: "id_rsa", destination: "$HOME/.ssh/id_rsa"
  #config.vm.provision "file", source: "id_rsa.pub", destination: "$HOME/.ssh/id_rsa.pub"
  #config.vm.provision "shell", inline: "cat /home/vagrant/.ssh/id_rsa.pub >> /home/vagrant/.ssh/authorized_keys"

  if Vagrant::Util::Platform.windows?
        # is windows
        config.vm.synced_folder ".", "/vagrant"
  else
        # is linux or some other OS
        config.vm.synced_folder ".", "/vagrant", type: "nfs"
  end

end
