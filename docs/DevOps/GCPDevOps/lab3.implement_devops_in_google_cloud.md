# LAB: Implement DevOps in Google Cloud

## 1. Google Cloud Source Repositories Setup

Google Cloud Source Repositories provides Git version control to support collaborative development of any application or service.

!!! abstract "Objectives"
    - create a local Git repository
    - add a Google Source Repository as a remote

- `gcloud source repos create` creates new repository
- `gcloud source repos clone` command add Google Cloud Source repository as remote origin.

```sh
gcloud source repos create REPO_DEMO
gcloud source repos clone REPO_DEMO

# normal git operations until committed a file
git push origin master

gcloud source repos list
```

## 2. Managing Deployments Using Kubernetes Engine

!!! abstract "Objectives"
    - Practice with kubectl tool
    - Create deployment yaml files
    - Launch, update, and scale deployments
    - Practice with updating deployments and deployment styles

### Create deployment 

🔘 Step 1: `kubectl explain` command can tell the detail of a **deployment object**.

```sh
kubectl explain deployment --recursive
kubectl explain deployment.metadata.name
```

🔘 Step 2: setting deployment configuration

Update the `deployments/auth.yaml` configuration file, setting the `auth` container to use image `"kelseyhightower/auth:1.0.0"`.

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: auth
spec:
  replicas: 1
  selector:
    matchLabels:
      app: auth
  template:
    metadata:
      labels:
        app: auth
        track: stable
    spec:
      containers:
        - name: auth
          image: "kelseyhightower/auth:1.0.0"
          ports:
            - name: http
              containerPort: 80
            - name: health
              containerPort: 81
...
```

🔘 Step 3: create deployment object and inspect

Use `kubectl create -f deployments/auth.yaml` to create deployment by the configuration file.
After successfully created, it can be inspected by `kubectl get deployments`.

A `ReplicaSet` will be created for the deployment, which can be seen by `kubectl get replicasets`.


🔘 Step 3: create service object

```sh
kubectl create -f services/auth.yaml

# create another deployment/service set.
kubectl create -f deployments/hello.yaml
kubectl create -f services/hello.yaml

# add secret/configmap and use it to create deployment/service
kubectl create secret generic tls-certs --from-file tls/
kubectl create configmap nginx-frontend-conf --from-file=nginx/frontend.conf
kubectl create -f deployments/frontend.yaml
kubectl create -f services/frontend.yaml
```


🔘 Step 5: Scale a deployment

Use `kubectl scale deploy` to scale a deployment, the number of replicas can be specified.

```sh
kubectl scale deployment hello --replicas=5
```

### Rolling update

When a deployment is updated with a new version, it creates **a new ReplicaSet** and slowly increases the number of replicas in the new ReplicaSet as it decreases the replicas in the old ReplicaSet.

🔘 Step 1: trigger rolling update.

When updating deployment using `kubectl edit deployment`, the rolling update can be triggered. 

```sh
kubectl edit deployment hello
kubectl get replicaset ## see the new replicaset
    #hello-5d9887b889      5         5         4       50s
    #hello-64fc59fc76      0         0         0       6m6s

kubectl rollout history deployment/hello ## set the rollout history

    # hello
    # deployment.apps/hello 
    # REVISION  CHANGE-CAUSE
    # 1         <none>
    # 2         <none>
```

🔘 Step 2: pause a rolling update

Rolling update can be paused.

```sh
kubectl rollout pause deployment/hello
kubectl rollout status deployment/hello
```

🔘 Step 3: resume a rolling update

The rollout is paused which means that some pods are at the **new** version and some pods are at the **older** version. The rolling process is resumed by: 

```sh
kubectl rollout resume deployment/hello
```

🔘 Step 4: rollback an update

`undo` command will help to rollback to last stage.

```sh
kubectl rollout undo deployment/hello
```

### Canary deployments

Canary deployments allow you to release a change to a **small subset** of your users to mitigate risk associated with new releases.
<div class="autocb" style="text-align:center;"><img src="./lab3.implement_devops_in_google_cloud.assets\autocb_0.png" style="zoom: 50%;box-shadow: rgba(0, 0, 0, 0.5) 10px 10px 10px; border-radius: 10px;" /></div>

In the `hello-canary.yaml`, specified the app name as `hello` but track as `canary`. This will create another deployment `hello-canary`. But in the service, it use `app:hello` selector which will match pods in both the prod deployment and canary deployment.  

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: hello-canary
spec:
  replicas: 1
  selector:
    matchLabels:
      app: hello
  template:
    metadata:
      labels:
        app: hello
        track: canary
        # Use ver 2.0.0 so it matches version on service selector
        version: 2.0.0
```

If want user to stay with same deployment, specified `sessionAffinity` in the `service` file.
All clients with the same IP address will have their requests sent to the same version of the hello application.

```yaml
kind: Service
apiVersion: v1
metadata:
  name: "hello"
spec:
  sessionAffinity: ClientIP
```

### Blue-green deployments

Some time we want load balancers to point to that new version **only** after it has been fully deployed. 

<div class="autocb" style="text-align:center;"><img src="./lab3.implement_devops_in_google_cloud.assets\autocb_1.png" style="zoom: 50%;box-shadow: rgba(0, 0, 0, 0.5) 10px 10px 10px; border-radius: 10px;" /></div>


Kubernetes achieves this by creating **two separate deployments**. Once the new "green" version is up and running, you'll switch over to using that version by **updating the Service**.

!!! note
    When using "blue-green" deployments, at least **2x** the resources in cluster is necessary.


## 3. Kubernetes Engine Pipeline using Cloud Build

!!! abstract "Objectives"
    - Create Kubernetes Engine clusters
    - Create Cloud Source Repositories
    - Trigger Cloud Build from Cloud Source Repositories
    - Automate tests and publish a deployable container image via Cloud Build
    - Manage resources deployed in a Kubernetes Engine cluster via Cloud Build

### Create the Git repositories in Cloud Source Repositories

🔘 Step 1: create two gi repositories in GC.

```sh
gcloud source repos create hello-cloudbuild-app
gcloud source repos create hello-cloudbuild-env
```

Use this repository as a base, add `hello-cloudbuild-app` repo as its remote.

```sh
git clone https://github.com/GoogleCloudPlatform/gke-gitops-tutorial-cloudbuild hello-cloudbuild-app

cd ~/hello-cloudbuild-app
# add REGION to each yaml file
sed -i "s/us-central1/$REGION/g" cloudbuild.yaml

git remote add google "https://source.developers.google.com/p/${PROJECT_ID}/r/hello-cloudbuild-app"
```

### Create a container image with Cloud Build

Using the following docker file to create a container.

```dockerfile
FROM python:3.7-slim
RUN pip install flask
WORKDIR /app
COPY app.py /app/app.py
ENTRYPOINT ["python"]
CMD ["/app/app.py"]
```

To build the container **with Cloud Build** and store it in **Artifact Registry**, using `gcloud builds`.

```sh
cd ~/hello-cloudbuild-app
COMMIT_ID="$(git rev-parse --short=7 HEAD)"
gcloud builds submit --tag="${REGION}-docker
```

### Create the Continuous Integration (CI) pipeline

🔘 Step1: create trigger for `hello-cloudbuild-app`, the event is `Push to a branch` and the build configuration is file `cloudbuild.yaml`.

🔘 Step2: trigger the event by push new content into `hello-cloudbuild-app`.

```sh
cd ~/hello-cloudbuild-app
git add .
git commit -m "Type Any Commit Message here"
git push google master
```

### Create the Test Environment and CD pipeline

In CD process, each time a commit is pushed to the `candidate` branch of the hello-cloudbuild-env repository, the pipeline **applies the new version of the manifest** to the Kubernetes cluster. If successful, itcopies the manifest over to the **production branch**.

- candidate branch -> deployment attempts
- production branch -> history of the successful deployments
- rollback to any previous deployment by re-executing the corresponding build in Cloud Build

🔘 Step1 : Grant Cloud Build access to GKE. IAM policy of **Kubernetes Engine Developer Identity** need to be added to cloudbuild.

```sh
gcloud projects add-iam-policy-binding ${PROJECT_NUMBER} \
--member=serviceAccount:${PROJECT_NUMBER}@cloudbuild.gserviceaccount.com \
--role=roles/container.developer
```

🔘 Step 2: create new branches production/candidate.

```sh
cd ~
gcloud source repos clone hello-cloudbuild-env
cd ~/hello-cloudbuild-env
git checkout -b production
git checkout -b candidate
```

Copy original cloudbuild file to `hello-cloudbuild-env` repo and commit.

```sh
cp ~/hello-cloudbuild-app/cloudbuild-delivery.yaml ~/hello-cloudbuild-env/cloudbuild.yaml
git add .
git commit -m "Create cloudbuild.yaml for deployment"
```

Similar to previous step, create trigger for `hello-cloudbuild-env`, set only `^candidate$` branch.