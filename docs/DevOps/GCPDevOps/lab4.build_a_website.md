# LAB: Build a Website on Google Cloud

## 1. Deploy Your Website on Cloud Run

!!! abstract "Objectives"
    - How to build a Docker image using Cloud Build and upload it to Artifact Registry
    - How to deploy Docker images to Cloud Run
    - How to manage Cloud Run deployments
    - How to setup an endpoint for an application on Cloud Run

<div class="autocb" style="text-align:center;"><img src="lab4.build_a_website.assets/autocb_0.png" style="zoom: 50%;box-shadow: rgba(0, 0, 0, 0.5) 10px 10px 10px; border-radius: 10px;" /></div>

🔘 Step1:  Clone the source repository

The website is a existed repository, first clone it and run it locally.

```sh
git clone https://github.com/googlecodelabs/monolith-to-microservices.git
cd ~/monolith-to-microservices
./setup.sh
cd ~/monolith-to-microservices/monolith
npm start
```

🔘 Step2: 