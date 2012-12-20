# Jenkins Build Monitoring

## Requirements

* python2.7
* python-serial

## Running

Upload the jenkins.ino sketch to your arduino. Plug your arduino in to the computer that will run the server. Point the [Jenkins Notification Plugin](https://wiki.jenkins-ci.org/display/JENKINS/Notification+Plugin) for your job to http://{your address}:41372/, and run:

```bash
$ python listener.py
```