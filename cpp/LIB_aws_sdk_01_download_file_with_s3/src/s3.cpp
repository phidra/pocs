#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>

#include <fstream>
#include <iostream>

namespace s3 {

std::string read_file(std::string bucket, std::string file) {
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    // Lorsqu'il est construit ainsi, le client S3 utiliser la chaîne de credentials par défaut :
    // https://sdk.amazonaws.com/cpp/api/LATEST/root/html/md_docs_2_credentials___providers.html
    Aws::S3::S3Client client{};

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.WithBucket(bucket).WithKey(file);

    auto outcome = client.GetObject(object_request);
    if (!outcome.IsSuccess()) {
        auto const& the_error = outcome.GetError();
        std::cout << "BOUM bucket='" << bucket << "' file='" << file << "' : " << the_error.GetExceptionName() << " "
                  << the_error.GetMessage() << std::endl;
        Aws::ShutdownAPI(options);
        std::exit(42);
    }

    Aws::S3::Model::GetObjectResult& result = outcome.GetResult();
    Aws::IOStream& response_body = result.GetBody();
    Aws::ShutdownAPI(options);

    std::stringstream ss;
    ss << response_body.rdbuf();
    return ss.str();
}

}  // namespace s3
