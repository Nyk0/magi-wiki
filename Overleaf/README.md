# Overleaf Installation Notes

The installation was performed with Overleaf toolkit. 

```console
root@overleaf:~# git clone https://github.com/overleaf/toolkit.git
```

We need to override the default volume configuration to attach them to an NFS share:

```console
root@overleaf:~# cat /opt/overleaf-toolkit/config/docker-compose.override.yml
volumes:
  overleaf-data:
    driver_opts:
      type: "nfs"
      o: "addr=<ip nfs server>,nolock,soft,rw"
      device: ":/overleaf_data/overleaf"
  mongo-data:
    driver_opts:
      type: "nfs"
      o: "addr=<ip nfs server>,nolock,soft,rw"
      device: ":/overleaf_data/mongo"
  redis-data:
    driver_opts:
      type: "nfs"
      o: "addr=<ip nfs server>,nolock,soft,rw"
      device: ":/overleaf_data/redis"

services:
  sharelatex:
    volumes:
      - overleaf-data:/var/lib/overleaf

  mongo:
    volumes:
      - mongo-data:/data/db

  redis:
    volumes:
      - redis-data:/data
```
We must fix some variables in overleaf.rc:

```console
root@overleaf:~# cat /opt/overleaf-toolkit/config/overleaf.rc
#### Overleaf RC ####

PROJECT_NAME=overleaf

# Sharelatex container
# Uncomment the OVERLEAF_IMAGE_NAME variable to use a user-defined image.
# OVERLEAF_IMAGE_NAME=sharelatex/sharelatex
OVERLEAF_DATA_PATH=data/overleaf
SERVER_PRO=false
OVERLEAF_LISTEN_IP=0.0.0.0
OVERLEAF_PORT=80

# Sibling Containers
SIBLING_CONTAINERS_ENABLED=false
DOCKER_SOCKET_PATH=/var/run/docker.sock

# Mongo configuration
MONGO_ENABLED=true
MONGO_DATA_PATH=data/mongo
MONGO_IMAGE=mongo
MONGO_VERSION=6.0

# Redis configuration
REDIS_ENABLED=true
REDIS_DATA_PATH=data/redis
REDIS_IMAGE=redis:6.2
REDIS_AOF_PERSISTENCE=true

# Git-bridge configuration (Server Pro only)
GIT_BRIDGE_ENABLED=false
GIT_BRIDGE_DATA_PATH=data/git-bridge

# TLS proxy configuration (optional)
# See documentation in doc/tls-proxy.md
NGINX_ENABLED=false
NGINX_CONFIG_PATH=config/nginx/nginx.conf
NGINX_HTTP_PORT=80
# Replace these IP addresses with the external IP address of your host
NGINX_HTTP_LISTEN_IP=127.0.1.1
NGINX_TLS_LISTEN_IP=127.0.1.1
TLS_PRIVATE_KEY_PATH=config/nginx/certs/overleaf_key.pem
TLS_CERTIFICATE_PATH=config/nginx/certs/overleaf_certificate.pem
TLS_PORT=443

# In Air-gapped setups, skip pulling images
# PULL_BEFORE_UPGRADE=false
# SIBLING_CONTAINERS_PULL=false
```

And also fix some variables in variables.env:

```console
root@overleaf:~# cat /opt/overleaf-toolkit/config/variables.env
OVERLEAF_APP_NAME="Overleaf USPN"

ENABLED_LINKED_FILE_TYPES=project_file,project_output_file

# Enables Thumbnail generation using ImageMagick
ENABLE_CONVERSIONS=true

# Disables email confirmation requirement
EMAIL_CONFIRMATION_DISABLED=true

## Nginx
# NGINX_WORKER_PROCESSES=4
# NGINX_WORKER_CONNECTIONS=768

## Set for TLS via nginx-proxy
# OVERLEAF_BEHIND_PROXY=true
# OVERLEAF_SECURE_COOKIE=true

OVERLEAF_SITE_URL=https://overleaf.univ-spn.fr
OVERLEAF_NAV_TITLE=Overleaf USPN
# OVERLEAF_HEADER_IMAGE_URL=http://somewhere.com/mylogo.png
OVERLEAF_ADMIN_EMAIL=dsi-recherche@univ-paris13.fr

# OVERLEAF_LEFT_FOOTER='[{"text": "Contact your support team", "url": "mailto:support@example.com"}]'
# OVERLEAF_RIGHT_FOOTER='[{"text": "Hello, I am on the Right"}]'

OVERLEAF_EMAIL_FROM_ADDRESS=dsi-recherche @_AT_@ univ-paris13.fr

# OVERLEAF_EMAIL_AWS_SES_ACCESS_KEY_ID=
# OVERLEAF_EMAIL_AWS_SES_SECRET_KEY=

OVERLEAF_EMAIL_SMTP_HOST=upn.univ-paris13.fr
OVERLEAF_EMAIL_SMTP_PORT=25
OVERLEAF_EMAIL_SMTP_SECURE=false
# OVERLEAF_EMAIL_SMTP_USER=
# OVERLEAF_EMAIL_SMTP_PASS=
# OVERLEAF_EMAIL_SMTP_NAME=
# OVERLEAF_EMAIL_SMTP_LOGGER=false
# OVERLEAF_EMAIL_SMTP_TLS_REJECT_UNAUTH=true
# OVERLEAF_EMAIL_SMTP_IGNORE_TLS=false
# OVERLEAF_CUSTOM_EMAIL_FOOTER=This system is run by department x

################
## Server Pro ##
################

EXTERNAL_AUTH=none
# OVERLEAF_LDAP_URL=ldap://ldap:389
# OVERLEAF_LDAP_SEARCH_BASE=ou=people,dc=planetexpress,dc=com
# OVERLEAF_LDAP_SEARCH_FILTER=(uid={{username}})
# OVERLEAF_LDAP_BIND_DN=cn=admin,dc=planetexpress,dc=com
# OVERLEAF_LDAP_BIND_CREDENTIALS=GoodNewsEveryone
# OVERLEAF_LDAP_EMAIL_ATT=mail
# OVERLEAF_LDAP_NAME_ATT=cn
# OVERLEAF_LDAP_LAST_NAME_ATT=sn
# OVERLEAF_LDAP_UPDATE_USER_DETAILS_ON_LOGIN=true

# OVERLEAF_TEMPLATES_USER_ID=578773160210479700917ee5
# OVERLEAF_NEW_PROJECT_TEMPLATE_LINKS=[{"name":"All Templates","url":"/templates/all"}]

# TEX_LIVE_DOCKER_IMAGE=quay.io/sharelatex/texlive-full:2022.1
# ALL_TEX_LIVE_DOCKER_IMAGES=quay.io/sharelatex/texlive-full:2022.1,quay.io/sharelatex/texlive-full:2021.1,quay.io/sharelatex/texlive-full:2020.1

# OVERLEAF_PROXY_LEARN=true

# S3
# Docs: https://github.com/overleaf/overleaf/wiki/S3
# ## Enable the s3 backend for filestore
# OVERLEAF_FILESTORE_BACKEND=s3
# ## Enable S3 backend for history
# OVERLEAF_HISTORY_BACKEND=s3
# #
# # Pick one of the two sections "AWS S3" or "Self-hosted S3".
# #
# # AWS S3
# ## Bucket name for project files
# OVERLEAF_FILESTORE_USER_FILES_BUCKET_NAME=overleaf-user-files
# ## Bucket name for template files
# OVERLEAF_FILESTORE_TEMPLATE_FILES_BUCKET_NAME=overleaf-template-files
# ## Key for filestore user
# OVERLEAF_FILESTORE_S3_ACCESS_KEY_ID=...
# ## Secret for filestore user
# OVERLEAF_FILESTORE_S3_SECRET_ACCESS_KEY=...
# ## Bucket region you picked when creating the buckets.
# OVERLEAF_FILESTORE_S3_REGION=""
# ## Bucket name for project history blobs
# OVERLEAF_HISTORY_PROJECT_BLOBS_BUCKET=overleaf-project-blobs
# ## Bucket name for history chunks
# OVERLEAF_HISTORY_CHUNKS_BUCKET=overleaf-chunks
# ## Key for history user
# OVERLEAF_HISTORY_S3_ACCESS_KEY_ID=...
# ## Secret for history user
# OVERLEAF_HISTORY_S3_SECRET_ACCESS_KEY=...
# ## Bucket region you picked when creating the buckets.
# OVERLEAF_HISTORY_S3_REGION=""
#
# # Self-hosted S3
# ## Bucket name for project files
# OVERLEAF_FILESTORE_USER_FILES_BUCKET_NAME=overleaf-user-files
# ## Bucket name for template files
# OVERLEAF_FILESTORE_TEMPLATE_FILES_BUCKET_NAME=overleaf-template-files
# ## Key for filestore user
# OVERLEAF_FILESTORE_S3_ACCESS_KEY_ID=...
# ## Secret for filestore user
# OVERLEAF_FILESTORE_S3_SECRET_ACCESS_KEY=...
# ## S3 provider endpoint
# OVERLEAF_FILESTORE_S3_ENDPOINT=http://10.10.10.10:9000
# ## Path style addressing of buckets. Most likely you need to set this to "true".
# OVERLEAF_FILESTORE_S3_PATH_STYLE="true"
# ## Bucket region. Most likely you do not need to configure this.
# OVERLEAF_FILESTORE_S3_REGION=""
# ## Bucket name for project history blobs
# OVERLEAF_HISTORY_PROJECT_BLOBS_BUCKET=overleaf-project-blobs
# ## Bucket name for history chunks
# OVERLEAF_HISTORY_CHUNKS_BUCKET=overleaf-chunks
# ## Key for history user
# OVERLEAF_HISTORY_S3_ACCESS_KEY_ID=...
# ## Secret for history user
# OVERLEAF_HISTORY_S3_SECRET_ACCESS_KEY=...
# ## S3 provider endpoint
# OVERLEAF_HISTORY_S3_ENDPOINT=http://10.10.10.10:9000
# ## Path style addressing of buckets. Most likely you need to set this to "true".
# OVERLEAF_HISTORY_S3_PATH_STYLE="true"
# ## Bucket region. Most likely you do not need to configure this.
# OVERLEAF_HISTORY_S3_REGION=""
```

When sharedlatex container is up and running, we must install all Latex packages:

```console
root@overleaf:~# cat /root/scripts/update-texlive.sh
#!/bin/bash

if docker ps --filter "status=running" --filter "name=^sharelatex$"; then
        docker exec sharelatex tlmgr update --self
        docker exec sharelatex tlmgr install scheme-full
fi
```


