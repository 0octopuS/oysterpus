# LAB: Automating Infrastructure on Google Cloud with Terraform

## 1. Terraform Fundamentals

Terraform is a tool for **building, changing, and versioning infrastructure** safely and efficiently. Key features: IaC, Execution plans, Resource graph, Change automation

!!! abstract "Objectives"
    - Get started with Terraform in Google Cloud.
    - Install Terraform from installation binaries.
    - Create a VM instance infrastructure using Terraform.

### Lab Step

🔘 Step 1: create `instance.tf` file as following.

It specifies creating a compute instance, naming "terraform. \
And for this instance, it specified its project, name, machine_type and other setting,
similar to the setting in Google Cloud Console.

```tf
resource "google_compute_instance" "terraform" {
  project      = "qwiklabs-gcp-01-ee27324076c2"
  name         = "terraform"
  machine_type = "e2-medium"
  zone         = "us-central1-f"

  boot_disk {
    initialize_params {
      image = "debian-cloud/debian-11"
    }
  }

  network_interface {
    network = "default"
    access_config {
    }
  }
}
```

🔘 Step 2: use terraform to plan

First initialize, then plan using the .tf file.

```shell
terraform init
terraform plan
```

The plan result is following:

```tex
terraform plan

Terraform used the selected providers to generate the following execution plan. Resource actions are indicated with the following symbols:
  + create

Terraform will perform the following actions:

  # google_compute_instance.terraform will be created
  + resource "google_compute_instance" "terraform" {
      + can_ip_forward       = false
      + cpu_platform         = (known after apply)
      + current_status       = (known after apply)
      + deletion_protection  = false
      + effective_labels     = (known after apply)
      + guest_accelerator    = (known after apply)
      + id                   = (known after apply)
      + instance_id          = (known after apply)
      + label_fingerprint    = (known after apply)
      + machine_type         = "e2-medium"
      + metadata_fingerprint = (known after apply)
      + min_cpu_platform     = (known after apply)
      + name                 = "terraform"
      + project              = "qwiklabs-gcp-01-ee27324076c2"
      + self_link            = (known after apply)
      + tags_fingerprint     = (known after apply)
      + terraform_labels     = (known after apply)
      + zone                 = "us-central1-f"

      + boot_disk {
          + auto_delete                = true
          + device_name                = (known after apply)
          + disk_encryption_key_sha256 = (known after apply)
          + kms_key_self_link          = (known after apply)
          + mode                       = "READ_WRITE"
          + source                     = (known after apply)

          + initialize_params {
              + image                  = "debian-cloud/debian-11"
              + labels                 = (known after apply)
              + provisioned_iops       = (known after apply)
              + provisioned_throughput = (known after apply)
              + size                   = (known after apply)
              + type                   = (known after apply)
            }
        }

      + network_interface {
          + internal_ipv6_prefix_length = (known after apply)
          + ipv6_access_type            = (known after apply)
          + ipv6_address                = (known after apply)
          + name                        = (known after apply)
          + network                     = "default"
          + network_ip                  = (known after apply)
          + stack_type                  = (known after apply)
          + subnetwork                  = (known after apply)
          + subnetwork_project          = (known after apply)

          + access_config {
              + nat_ip       = (known after apply)
              + network_tier = (known after apply)
            }
        }
    }

Plan: 1 to add, 0 to change, 0 to destroy.
```

🔘 Step 3: apply the change

If the plan was created successfully, Terraform will now pause and wait for approval before proceeding. Use `terraform apply` to apply the .tf file.
Inspect the state by `terraform show`.

## 2. Infrastructure as Code with Terraform

!!! abstract "Objectives"
    - Build, change, and destroy infrastructure with Terraform
    - Create Resource Dependencies with Terraform
    - Provision infrastructure with Terraform

### Lab Steps

🔘 Step 1: Build infrastructure

```sh
touch main.tf
```

🔘 Step :
🔘 Step :
🔘 Step :
🔘 Step :

## 3. Interact with Terraform Modules

A Terraform module is **a set of Terraform configuration files** in a single directory. Even a simple configuration consisting of a single directory with one or more .tf files is a module.

!!! abstract "Objectives"
    - Use a module from the Registry
    - Build a module

The structure usually like

``` shell
├── LICENSE
├── README.md
├── main.tf
├── variables.tf
├── outputs.tf
```

### Using module from registry

🔘 `main.tf` contains a `module`, specified the source from terraform registry.

`module` "test-vpc-module" defines a Virtual Private Cloud (VPC), which will provide networking services for the rest of your infrastructure.

```HCL
module "test-vpc-module" {
  source       = "terraform-google-modules/network/google"
  version      = "~> 6.0"
  project_id   = var.project_id # Replace this with your project ID
  network_name = "my-custom-mode-network"
  mtu          = 1460

  subnets = [{subnet_name   = "subnet-01"...}, {...}, ...]

}
```

🔘 `variable.tf` contains variables with their description, default value and so on.

These variable can be used in other file by `var.variable_name`.

```hcl
variable "project_id" {
  description = "The project ID to host the network in"
  default     = "FILL IN YOUR PROJECT ID HERE"
}
```

🔘 `output.tf` contains the output value by the module, which can be accessed by referring to `module.<MODULE NAME>.<OUTPUT NAME>`.

```hcl
output "network_name" {
  value       = module.test-vpc-module.network_name
  description = "The name of the VPC being created"
}
```

### Define own module

Define the following file structure:

```sh
main.tf
outputs.tf
variables.tf
modules/
└── gcs-static-website-bucket
    ├── LICENSE
    ├── README.md
    ├── website.tf
    ├── outputs.tf
    └── variables.tf
```

🔘 `modules/website.tf` defines the `resource`

```hcl
resource "google_storage_bucket" "bucket" {
}
```

🔘 `modules/variable.tf` and `modules/output.tf` are similar to previous file. There is no `main.tf` file in the module.

🔘 `main.tf` can use module by referring it.

```hcl
module "gcs-static-website-bucket" {
source = "./modules/gcs-static-website-bucket"
...
}
```

🔘 `output.tf` can access to the output of the module by

```HCL
output "bucket-name" {
  description = "Bucket names."
  value       = "module.gcs-static-website-bucket.bucket"
}
```

## 4. Managing Terraform State

State is a **necessary requirement** for Terraform to function. 

!!! abstract "Objectives"
    - Create a local backend.
    - Create a Cloud Storage backend.
    - Refresh your Terraform state.
    - Import a Terraform configuration.
    - Manage the imported configuration with Terraform.

!!! question "Why states?"
    - tracking the mappings between resources and remote objects(one remote object, one resource instance)
    - track metadata such as resource dependencies(delete order)
    - perform improvement, terraform stores a cache of the attribute values for all resources in the state
    - sync for team use
    - state locking to prevent file corrupt

### Working with backends

A **backend** in Terraform determines how state is loaded and how an operation such as apply is executed.


### Import Terraform configuration
