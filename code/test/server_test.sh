#!/bin/bash

curl --silent --output /dev/null "192.168.0.30:8080/start"
for i in {1..100}
do
  curl --silent --output /dev/null "192.168.0.30:8080/power?pwr=${i}" &
  curl --silent --output /dev/null "192.168.0.30:8080/orientation?ori=${i}" &
done

curl --silent --output /dev/null "192.168.0.30:8080/stop"
curl --silent --output /dev/null "192.168.0.30:8080/abort"
