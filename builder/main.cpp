// Copyright 2022 Shevelyova Darya photodoshfy@gmail.com

#include <builder.hpp>

int main(int argc, const char *argv[]) {
  try
  {
    boost::program_options::options_description desc{"Options"};
    boost::program_options::variables_map vmap;
    Builder::create_program_options(desc, vmap, argc, argv);

    if(vmap.count("help"))
      std::cout << desc;
    else if(argc > 1){
      Builder builder{};
      builder.start(vmap);
    } else
      throw boost::program_options::error(error);
  } catch (const boost::program_options::error &e)
  {
    BOOST_LOG_TRIVIAL(error) << e.what() << '\n';
  }
}