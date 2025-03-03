我在四个docker容器（镜像来自）中分别测试SwitchML以及NCCL在四个节点上的Allreduce性能，具体地
所有实验的启动容器的命令如下：
```
sudo docker run -dit --gpus device=0 \
            --net=host --cap-add=IPC_LOCK \
            --shm-size=32768m \
            --cap-add SYS_ADMIN \
            --cap-add SYS_RESOURCE  \
            --device=/dev/infiniband/uverbs1 \
            -v /home/yanhl/switchml_shared:/shared \
            --name switchml-rdma0 \
            crpi-iyh7vkeseh80me1w.cn-shenzhen.personal.cr.aliyuncs.com/yanhaolin/switchml:r0.0.5
```

我主要修改了switchml.cfg中的以下几个参数：
```
...
num_workers = 4
num_worker_threads = 8
max_outstanding_packets = 256
packet_numel = 64
backend = rdma
prepostprocessor = bypass 
msg_numel = 64
```
编译benchmark的命令如下：
```
make clean
make allreduce_benchmark RDMA=1 VCL=1 DEBUG=0 CUDA=1
```
运行benchmark的命令如下，每次Allreduce 4MB数据
```
./bin/allreduce_benchmark --tensor-numel=1048576 --tensor-type=int32 --device=gpu
```
结果非常不理想
```
Signal handler thread started. Waiting for any signals.
Submitting 5 warmup jobs.
Warmup finished.
Submitting 10 jobs.
Job(s) #0# finished. Duration: #531972764# ns Goodput: #0.0630755# Gbps.
Job(s) #1# finished. Duration: #539886204# ns Goodput: #0.0621509# Gbps.
Job(s) #2# finished. Duration: #575978341# ns Goodput: #0.0582564# Gbps.
Job(s) #3# finished. Duration: #567985377# ns Goodput: #0.0590762# Gbps.
Job(s) #4# finished. Duration: #587951282# ns Goodput: #0.0570701# Gbps.
Job(s) #5# finished. Duration: #572039624# ns Goodput: #0.0586575# Gbps.
Job(s) #6# finished. Duration: #560061712# ns Goodput: #0.059912# Gbps.
Job(s) #7# finished. Duration: #575842023# ns Goodput: #0.0582702# Gbps.
Job(s) #8# finished. Duration: #583945036# ns Goodput: #0.0574616# Gbps.
Job(s) #9# finished. Duration: #556026482# ns Goodput: #0.0603468# Gbps.
All jobs finished.


Min 531972764 ns 0.0630755 Gbps
Max 587951282 ns 0.0570701 Gbps
Median 572039624 ns 0.0586575 Gbps
Mean 565168884 ns 0.0593706 Gbps
Std dev 1.73446e+07 ns
Cleaning up.
Signal handler thread is exiting
```









> xurtyf-topxip-xYpjo4
P4编译
基准测试

root@216DGX1:/home/switchml/dev_root/build# ./bin/allreduce_benchmark --tensor-numel=1048576 --tensor-type=int32
Signal handler thread started. Waiting for any signals.
Submitting 5 warmup jobs.
Warmup finished.
Submitting 10 jobs.
Job(s) #0# finished. Duration: #247930194# ns Goodput: #0.135338# Gbps.
Job(s) #1# finished. Duration: #247894665# ns Goodput: #0.135358# Gbps.
Job(s) #2# finished. Duration: #232051864# ns Goodput: #0.144599# Gbps.
Job(s) #3# finished. Duration: #227914354# ns Goodput: #0.147224# Gbps.
Job(s) #4# finished. Duration: #232015511# ns Goodput: #0.144622# Gbps.
Job(s) #5# finished. Duration: #236068173# ns Goodput: #0.142139# Gbps.
Job(s) #6# finished. Duration: #235874039# ns Goodput: #0.142256# Gbps.
Job(s) #7# finished. Duration: #251944270# ns Goodput: #0.133182# Gbps.
Job(s) #8# finished. Duration: #240000825# ns Goodput: #0.13981# Gbps.
Job(s) #9# finished. Duration: #236010404# ns Goodput: #0.142174# Gbps.
All jobs finished.


./bin/allreduce_benchmark --tensor-numel=1048576 --tensor-type=int32


# 大页内存测试
cd checks
g++ -o hugetlbfs_test hugetlbfs_test.cc -lhugetlbfs
./hugetlbfs_test
# 关闭检查
cd dev_root/scripts
sudo sh disable-icrc.sh
# 启动容器

xurtyf-topxip-xYpjo4


sudo docker run -dit --gpus device=0 \
            --net=host --cap-add=IPC_LOCK \
            --shm-size=32768m \
            --cap-add SYS_ADMIN \
            --cap-add SYS_RESOURCE  \
            --device=/dev/infiniband/uverbs1 \
            -v /home/yanhl/switchml_shared:/shared \
            --name switchml-rdma0 \
            crpi-iyh7vkeseh80me1w.cn-shenzhen.personal.cr.aliyuncs.com/yanhaolin/switchml:r0.0.5


sudo docker run -dit --gpus device=1 \
            --net=host --cap-add=IPC_LOCK \
            --shm-size=32768m \
            --cap-add SYS_ADMIN \
            --cap-add SYS_RESOURCE  \
            --device=/dev/infiniband/uverbs2 \
            -v /home/yanhl/switchml_shared:/shared \
            --name switchml-rdma1 \
            crpi-iyh7vkeseh80me1w.cn-shenzhen.personal.cr.aliyuncs.com/yanhaolin/switchml:r0.0.5

sudo docker run -dit --gpus device=2 \
            --net=host --cap-add=IPC_LOCK \
            --shm-size=32768m \
            --cap-add SYS_ADMIN \
            --cap-add SYS_RESOURCE  \
            --device=/dev/infiniband/uverbs3 \
            -v /home/yanhl/switchml_shared:/shared \
            --name switchml-rdma2 \
            crpi-iyh7vkeseh80me1w.cn-shenzhen.personal.cr.aliyuncs.com/yanhaolin/switchml:r0.0.5

sudo docker run -dit --gpus device=3 \
            --net=host --cap-add=IPC_LOCK \
            --shm-size=32768m \
            --cap-add SYS_ADMIN \
            --cap-add SYS_RESOURCE  \
            --device=/dev/infiniband/uverbs4 \
            -v /home/yanhl/switchml_shared:/shared \
            --name switchml-rdma3 \
            crpi-iyh7vkeseh80me1w.cn-shenzhen.personal.cr.aliyuncs.com/yanhaolin/switchml:r0.0.5




sudo docker exec -it switchml-rdma1 bash


sudo docker run -it --gpus all --net=host --cap-add=IPC_LOCK --device=/dev/infiniband/uverbs2 -v /home/yanhl/switchml_shared:/shared --name switchml-rdma2 crpi-iyh7vkeseh80me1w.cn-shenzhen.personal.cr.aliyuncs.com/yanhaolin/switchml:v0.0.4



sudo bash setup_containers.sh 2 /home/yanhl/shared 172.22.10.10 switchml:u18.04-cu11.2
sudo docker exec -it deepspeed-worker1 bash
cd /home/switchml/dev_root/build

# P4
export YHL_HOME=/root/bf-sde-9.2.0/pkgsrc/p4-examples/p4_16_programs/yhl
python3 switchml.py --program=switchml


$SDE/pkgsrc/p4-build/configure --with-tofino --with-p4c=p4c --prefix=$SDE_INSTALL \
      --bindir=$SDE_INSTALL/bin \
      P4_NAME=switchml \
      P4_PATH=$SDE/pkgsrc/p4-examples/p4_16_programs/yhl/p4/switchml.p4 \
      P4_VERSION=p4-16 P4_ARCHITECTURE=tna \
      P4FLAGS="--verbose 2 --create-graphs -g" \
      LDFLAGS="-L$SDE_INSTALL/lib"


```

$SDE_INSTALL/bin/bf_kdrv_mod_load $SDE_INSTALL/
export YHL_HOME=/root/bf-sde-9.2.0/pkgsrc/p4-examples/p4_16_programs/yhl
cd $SDE/build/p4-build/yhl_l3_forward/
$SDE/pkgsrc/p4-build/configure --with-tofino --with-p4c=p4c --prefix=$SDE_INSTALL \
      --bindir=$SDE_INSTALL/bin \
      P4_NAME=yhl_l3_forward \
      P4_PATH=$SDE/pkgsrc/p4-examples/p4_16_programs/yhl/p4/yhl_l3_forward.p4 \
      P4_VERSION=p4-16 P4_ARCHITECTURE=tna \
      P4FLAGS="--verbose 2 --create-graphs -g" \
      LDFLAGS="-L$SDE_INSTALL/lib"
make
make install 

./run_p4_tests.sh -p yhl_l3_forward -t $PROJECT_PATH/tests/


/root/bf-sde-9.10.0/install/bin/p4_build.sh 

P4_NAME=switchml \
P4FLAGS="--verbose 2 --create-graphs -g" \
```


/root/bf-sde-9.10.0/install/bin/p4_build.sh --with-tofino --with-p4c=p4c \
      /root/bf-sde-9.10.0/yhl_dev/p4/switchml.p4 \
      P4_NAME=switchml \
      P4_VERSION=p4-16 P4_ARCHITECTURE=tna \
      P4FLAGS="--verbose 2 --create-graphs -g" \
      LDFLAGS="-L$SDE_INSTALL/lib"

/root/bf-sde-9.10.0/install/bin/p4_build.sh \
      ../p4/switchml.p4  P4_VERSION=p4_16 P4_ARCHITECTURE=tna \
      P4FLAGS="--verbose 2 --create-graphs -g" \
      LDFLAGS="-L$SDE_INSTALL/lib"