#### Instruction

      cmake .
      make
      ./build/bin/TEST
      
 #### Output
 
            uri_builder => http://test.com/nnrf-disc/v1/nf-instances?requester-nf-type=%22AMF%22&services=%5B%22nudm-uecm%22%5D&snssais=%5B%7B%22sd%22:%22FFFFFF%22,%22sst%22:1%7D%5D&target-nf-type=%22SMF%22

            uri => http://test.com/nnrf-disc/v1/nf-instances?requester-nf-type=%22AMF%22&services=%5B%22nudm-uecm%22%5D&snssais=%5B%7B%22sd%22:%22FFFFFF%22,%22sst%22:1%7D%5D&target-nf-type=%22SMF%22

            requester-nf-type AMF
            services nudm-uecm
            snssais sd = FFFFFF sst = 1
            target-nf-type SMF

      
      
