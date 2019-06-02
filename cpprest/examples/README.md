#### If you have existing apt packages installed for cpprestsdk, please remove those using the following commands.


        sudo apt-get purge libcpprest-dev
        sudo apt-get remove libcpprest-dev
        sudo apt autoremove

--------------

      sudo apt-get install g++ git libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev libwebsocketpp-dev openssl libssl-dev ninja-build

      git clone https://github.com/Microsoft/cpprestsdk.git casablanca

      cd casablanca
	    mkdir build.debug
	    cd build.debug
	    cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Debug
	    ninja

#### 4.	  

      (optional test check,needs internet connection)	
	    cd Release/Binaries
	    ./test_runner *_test.so

#### 5.
      sudo ninja install (inside /casablanca/build.debug/)

#### 6. 	echo $LD_LIBRARY_PATH (optional, setting this would work for only this shell session)
	(if the above output does not inclide casablanca Release/Binaries , then do the following)
	
	    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<path to buld.debug/Release/Binaries>
	
  (for example path could be /home/deka/Desktop/casablanca/build.debug/Release/Binaries )

#### 7.	Install created binaries in /usr/lib/
	
      unset LD_LIBRARY_PATH (if you have done step 6)
      sudo ldconfig
      sudo /home/deka/Desktop/casablanca/build.debug/Release/Binaries/* /usr/lib
      (replace the path, with your path to binaries)

--------------

#### After successfull installation, we can the following code snippet to set the number of worker threads of casablanca.

      crossplat::threadpool::initialize_with_threads(200 or 300 etc);
