

        ################################################################
        #
        # Lambda Calculus demonstration in Perl
        # 4 April 1999 
        # M-J. Dominus (mjd-perl-lc@plover.com)
        # http://www.plover.com/~mjd/perl/lambda/
        #

        ############
        #
        # Truth and truth testing

        # IF = %btf.btf
        $IF = sub { my $b = shift;
                    sub { my $t = shift;
                          sub { my $f = shift;
                                ($b)->($t)->($f);
                              }
                        }
                  }
        ;
        
        # TRUE = %xy.x
        $TRUE = sub { my $x = shift;
                      sub { my $y = shift;
                            $x;
                          }
                    }
        ;
        
        # FALSE = %xy.y
        $FALSE = sub { my $x = shift;
                       sub { my $y = shift;
                             $y;
                           }
                     }
        ;

        ############
        #
        # Pairs (conses)
        
        # PAIR = %xyb.bxy
        $PAIR = sub { my $x = shift;
                      sub { my $y = shift;
                            sub { my $b = shift;
                                  ($b)->($x)->($y);
                                }
                          }
                    }
        ;
        
        # FIRST = %p.(p TRUE)
        $FIRST = sub { my $p = shift;
                       ($p)->($TRUE);
                     }
        ;
        
        # SECOND = %p.(p FALSE)
        $SECOND = sub { my $p = shift;
                        ($p)->($FALSE);
                      }
        ;
        
        
        ############
        #
        # Church Numerals
        #

	# 0 = %f.%x.x
	$ZERO = sub { my $f = shift;
		      sub { my $x = shift;
			    $x
			  }
		    }
	;

	# SUCC = %n.%f.%x.f (n f x)
	$SUCC = sub { my $n = shift;
		      sub { my $f = shift;
			    sub { my $x = shift;
				  $f->($n->($f)->($x));
				}
			  }
		    }
	;

	# ONE = SUCC 0
	$ONE = $SUCC->($ZERO);

	# ADD = %mn.%f.%x.(m f) (n f x)
	$ADD = sub { my $m = shift;
		     sub { my $n = shift;
			   sub { my $f = shift;
				 sub { my $x = shift;
				       ($m->($f))->($n->($f)->($x));
				     }
			       }
			 }
		   }
	;

	# IS_ZERO = %n.(n (%k.FALSE) TRUE)
	$IS_ZERO = sub { my $n = shift;
			 $n->(sub { my $k = shift; $FALSE })->($TRUE);
		       }
	;

        ############
        #
        # We use this utility function only for checking to see if the
        # addition was done correctly
        #       

	sub convert_to_perl_number {
	  my $n = shift;
	  $n->(sub {my $n = shift; $n+1})->(0);
	}

	$mystery_result = $ADD->($ONE)->($ONE);
	print "1+1 = ", convert_to_perl_number($mystery_result), "\n";


